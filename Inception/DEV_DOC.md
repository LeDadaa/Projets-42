1) Environment setup (from scratch)

- Install prerequisites on the host:

sudo apt update
sudo apt install -y docker.io docker-compose-plugin curl
sudo usermod -aG docker $USER

- Create or edit the environment file used by Docker Compose: `srcs/.env`.
	Put database and WordPress credentials there (example variables are already in `srcs/.env`).

- The Makefile uses two host folders for persistent data. By default they are:
	- `/home/damalca/data/wordpress` for WordPress files
	- `/home/damalca/data/mariadb` for database files
	You can change these paths by editing the `WP_DATA` and `DB_DATA` variables in the `Makefile`.

2) Build and launch the project (Makefile + Docker Compose)

- From the project root, run the simple target to build and start everything:

make up

- Other useful Makefile targets:

make build      # build images
make start      # start stopped containers
make stop       # stop containers
make down       # stop and remove containers
make clean      # remove images, containers, volumes and host data directories
make prune      # full cleanup (prune system)

All Makefile targets call `docker compose -f srcs/docker-compose.yml ...` so you can also run Docker Compose commands directly if you prefer.

3) Manage containers, logs and volumes

- See running services:

docker compose -f srcs/docker-compose.yml ps

- Follow logs:

docker compose -f srcs/docker-compose.yml logs -f

- List Docker volumes and networks:

docker volume ls
docker network ls

4) Where project data is stored and how it persists

- The project uses named Docker volumes mapped to host folders. Check `srcs/docker-compose.yml` — by default:
	- WordPress files are stored on the host at `/home/damalca/data/wordpress` (mounted to `/var/www/wordpress` in containers).
	- MariaDB data is stored at `/home/damalca/data/mariadb` (mounted to `/var/lib/mysql`).

- Because volumes are bound to host paths, data persists across container rebuilds and restarts. To fully remove persistent data, use `make clean` or delete the host directories shown above.

5) Quick tips

- If you change `srcs/.env`, re-create containers to apply changes:

make down
make up