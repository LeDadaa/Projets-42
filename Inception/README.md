*This project has been created as part of the 42 curriculum by <damalca>*

## Description

This project builds a small web stack using Docker containers. The goal is to set up a working web service (Nginx + WordPress + MariaDB) that is easy to run and test.

## Instructions

- Build and start the project with Docker Compose:

make up

- Stop and remove containers:

make down

## Resources

- Docker documentation: https://docs.docker.com/
- Docker Compose: https://docs.docker.com/compose/
- Nginx: https://nginx.org/
- MariaDB: https://mariadb.org/
- WordPress: https://wordpress.org/
- Some more forums and docker explanations helped me through the project

AI usage: AI assisted me with writing this README properly and added some usefull informations in the 3 mds files

## Project description

This project uses Docker to run each service in its own container. The repository includes Dockerfiles and configuration under `srcs/` for:

- `nginx/` : Nginx configuration and Dockerfile
- `mariadb/` : MariaDB Dockerfile and setup script
- `wordpress/` : WordPress Dockerfile and setup script
- `docker-compose.yml` : Compose file that links services and sets networks/volumes

Main design choices:

- Use Docker Compose to manage multiple containers.

Comparisons:

- Virtual Machines vs Docker: Docker is lighter and starts faster because it shares the host kernel; VMs provide full isolation but use more resources.
- Secrets vs Environment Variables: Secrets are safer for sensitive data because they avoid exposing values in process lists or in plain files; environment variables are simpler for development but less secure.
- Docker Network vs Host Network: Docker networks isolate container traffic and allow controlled communication; host network gives containers full access to the host network stack (less isolated, can be simpler for some setups).
- Docker Volumes vs Bind Mounts: Volumes are managed by Docker and are good for persistent data; bind mounts map host folders directly, which is useful for development but can be less portable.

verifier https

curl -I http://damalca.42.fr
curl -k -I https://damalca.42.fr

verifier que le port nginx est disponible apres modification

ss -ltn | grep :8443
make down
make up
curl -k https://damalca.42.fr:8443

DATABASE mariadb

sudo docker compose exec mariadb \ mariadb -h 127.0.0.1 -u root -fail
sudo docker compose exec mariadb \ mariadb-h 127.0.0.1 \ -u database_user \ -p'database_user_password' \ database_name -valid