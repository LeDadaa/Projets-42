"""avatars

3 rules

1. start refuse while loading `await file.read()`
   if file is 2 Go for instance, leads to RDOS request denial of service
2. do not trust `content_type` or extension
3. Re encode : a GIF which is also HTML containing JS
   decoded then reencoded remains poxels
   also deletes metadata which is good
"""

from __future__ import annotations

import asyncio
import colorsys
import hashlib
import io
import secrets
from pathlib import Path

from fastapi import HTTPException, UploadFile, status
from PIL import Image, UnidentifiedImageError

MEDIA_ROOT = Path("/srv/media")
MAX_AVATAR_BYTES = 2 * 1024 * 1024  # 2 Mo —consistent with client_max_body_size 2m
CHUNK = 64 * 1024
ALLOWED_FORMATS = frozenset({"JPEG", "PNG", "WEBP", "GIF"})
AVATAR_SIZE = 512

# Decompression : 4ko PNG can be 50 000 x 50 000 pixels = ~7,5 Go after decode
# Pillow raise DecompressionBombError
Image.MAX_IMAGE_PIXELS = 40_000_000


async def read_limited(file: UploadFile, limit: int = MAX_AVATAR_BYTES) -> bytes:
    """Reads an upload but stops the moment it goes over the size cap"""
    buf = bytearray()
    while chunk := await file.read(CHUNK):
        buf.extend(chunk)
        if len(buf) > limit:
            raise HTTPException(
                status.HTTP_413_CONTENT_TOO_LARGE,
                detail=f"file too heavy (max {limit // 1024 // 1024} Mo)",
            )
    return bytes(buf)


def _process_sync(raw: bytes) -> bytes:
    """Validates then re encodes an image the blocking way

    415 if Pillow cannot read it or the format is not allowed
    """
    # Everything Pillow touches is attacker controlled, so it all lives inside the try
    # The 415 raised below crosses it untouched: HTTPException inherits
    # from neither OSError nor ValueError
    try:
        # 1st pass checks `verify()` truncated or malformed images
        with Image.open(io.BytesIO(raw)) as img:
            # `img.format` comes from decoded by Pillow
            fmt = img.format
            if fmt not in ALLOWED_FORMATS:
                raise HTTPException(
                    status.HTTP_415_UNSUPPORTED_MEDIA_TYPE,
                    detail=f"unauthorised format : {fmt}",
                )
            img.verify()

        # 2nd pass : transform
        with Image.open(io.BytesIO(raw)) as img:
            rgb = img.convert("RGB")
            rgb.thumbnail((AVATAR_SIZE, AVATAR_SIZE), Image.Resampling.LANCZOS)
            out = io.BytesIO()
            rgb.save(out, format="WEBP", quality=82, method=4)
            return out.getvalue()

    # Before the other except : DecompressionBombError derives from Exception,
    # not from OSError, so it would never be reached after it
    except Image.DecompressionBombError as exc:
        # 4 kB of PNG declaring 50 000² pixels : refused at decode, not at size
        raise HTTPException(
            status.HTTP_413_CONTENT_TOO_LARGE,
            detail="image too large once decoded",
        ) from exc
    # UnidentifiedImageError derives from OSError, named for what it says
    # a polyglot `GIF89a<script>` lands here : Pillow never identifies it,
    # so the whitelist above never even runs. Without this the request is a 500
    except (UnidentifiedImageError, OSError, ValueError) as exc:
        raise HTTPException(
            status.HTTP_415_UNSUPPORTED_MEDIA_TYPE,
            detail="unreadable image",
        ) from exc


async def process_avatar(raw: bytes) -> bytes:
    """Validates and re encodes an uploaded image

    Pillow is pure CPU so out of event loop
    a 4000×3000 image  can take 200 ms."""
    return await asyncio.to_thread(_process_sync, raw)


async def save_avatar(data: bytes) -> str:
    """Writes the processed image under a random name

    - a client name can chose a path (`../../etc/passwd`),
    - delete another user file
    - inject through the name
    """
    name = f"{secrets.token_hex(16)}.webp"
    path = MEDIA_ROOT / "avatars" / name
    await asyncio.to_thread(path.parent.mkdir, parents=True, exist_ok=True)
    await asyncio.to_thread(path.write_bytes, data)
    return f"/media/avatars/{name}"  # public path, nginx location /media/


def identicon(user_id: int, size: int = AVATAR_SIZE) -> bytes:
    """Draws a fallback avatar from a user id

    5×5 symetric like comme GitHub
    id hash gives color and shape, nothing to store or regenerate
    """
    digest = hashlib.sha256(str(user_id).encode()).digest()
    r, g, b = (int(c * 255) for c in colorsys.hsv_to_rgb(digest[0] / 255.0, 0.55, 0.72))

    grid = 5
    img = Image.new("RGB", (grid, grid), (238, 238, 243))
    for x in range(3):  # half left + center
        for y in range(grid):
            if digest[1 + x * grid + y] & 1:
                img.putpixel((x, y), (r, g, b))
                img.putpixel((grid - 1 - x, y), (r, g, b))  # mirror
    out = io.BytesIO()
    img.resize((size, size), Image.Resampling.NEAREST).save(out, format="WEBP")
    return out.getvalue()
