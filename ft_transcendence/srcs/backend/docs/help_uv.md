## Required:
- python (last)
- uv `curl -LsSf https://astral.sh/uv/install.sh | sh`
- docker + docker compose
- postgresql `sudo apt install postgresql`

## Useful :
- vscode extension `ruff`

## UV initiation and commands:
- `uv init` initiate pyproject.toml
- `uv add fastapi uvicorn` add a dependency (pyproject + uv.lock)
- `uv add --dev pytest`	not done so far but if dev dependency only

- `uv sync`	**TO BE DONE BY EVRYONE** install what's said by uv.lock
- `uv run uvicorn app.main:app --reload` exec in venv ?
- `uv lock --upgrade` update uv.lock
