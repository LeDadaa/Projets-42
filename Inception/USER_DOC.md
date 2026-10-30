User Documentation

This document explains in simple terms how to use the project stack.

1) What services are provided

- A web server (Nginx) to serve the site.
- A database server (MariaDB) to store data.
- WordPress as the web application (site + admin panel).

2) Start and stop the project

- Start and build containers:

make up

- Stop and remove containers:

make down

3) Access the website and admin panel

- Website: open `https://damalca.42.fr` in a browser
- Admin panel: open `https://damalca.42.fr/wp-admin` and log in with the WordPress admin account.

4) Locate and manage credentials

- Credentials are set in `docker-compose.yml` and in setup scripts under `srcs/`:
	- Check `srcs/mariadb/mdb-conf.sh` for database user/password setup.
	- Check `srcs/wordpress/wp_conf.sh` for WordPress credentials and configuration.
- To change a credential: update the environment variables in `docker-compose.yml` or the scripts, then run:

make up

5) Check that services are running correctly

- See running containers:

docker-compose ps

- See Docker containers list:

docker ps

- Test the web server from the host:

curl -I https://localhost