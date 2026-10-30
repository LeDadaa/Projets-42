# Permissions table

Expected status
`401` = not authenticated
`403` = role insufficient
`404` = exists but not visible for this role

| Route | anonymous | student | author | moderator | admin |
|---|---|---|---|---|---|
| `GET /auth/me` | 401 | 200 | 200 | 200 | 200 |
| `PATCH /users/me` | 401 | 200 | 200 | 200 | 200 |
| `POST /users/me/avatar` | 401 | 200 | 200 | 200 | 200 |
| `GET /friends` | 401 | 200 | 200 | 200 | 200 |
| `POST /friends/requests` | 401 | 201 | 201 | 201 | 201 |
| `GET /questions` (published) | 200 | 200 | 200 | 200 | 200 |
| `GET /questions?status=draft` | 403 | 403 | 403 | 200 | 200 |
| `POST /questions` | 401 | 403 | 201 | 201 | 201 |
| `PATCH /questions/{id}` (own, draft) | 401 | 403 | 200 | 200 | 200 |
| `PATCH /questions/{id}` (someone else's) | 401 | 403 | **403** | 200 | 200 |
| `PATCH /questions/{id}` (own, published) | 401 | 403 | **409** | 200 | 200 |
| `POST /questions/{id}/submit` | 401 | 403 | 200 | 200 | 200 |
| `POST /questions/{id}/approve` | 401 | 403 | **403** | 200 | 200 |
| `POST /questions/{id}/reject` | 401 | 403 | 403 | 200 | 200 |
| `GET /moderation/queue` | 401 | 403 | 403 | 200 | 200 |
| `GET /admin/users` | 401 | 403 | 403 | 403 | 200 |
| `PATCH /admin/users/{id}/role` | 401 | 403 | 403 | 403 | 200 |
| `GET /admin/audit-log` | 401 | 403 | 403 | 403 | 200 |
