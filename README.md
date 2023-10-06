# Instructions for setting up a PostgreSQL server in an Ubuntu 22.04 container.

If you prefer not to download and install the cluster manually,
pull the Docker image with running PostgreSQL from this URL:
https://hub.docker.com/repository/docker/ilidock95/pg_bootcamp_5_oct

After pulling the image, jump to [7. Work with PostgreSQL's Extensions'](#7-work-with-postgresqls-extensions).

# 1. Install Docker if not already installed

Update the package list in Ubuntu
```bash
sudo apt-get update 
```
 Install packages for Docker:
 - ca-certificates: for HTTPS
 - curl: for sending data
 - gnupg: for GPG's keys
 - lsb-release: find out Ubuntu's version
 ```bash
sudo apt-get -y install ca-certificates curl gnupg lsb-release
```
 Creating directory for storing GPD Docker's key
 ```bash
sudo mkdir -p /etc/apt/keyrings
```
 Loading GPG Docker's and adding it in key's apt
 ```bash
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
```
 Adding Docker's repository APT source's list
 This is for installing Docker packages in Ubuntu from official Docker's repository
 ```bash
echo \
	"deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list >/dev/null
```
 Updating Docker's packages
 ```bash
sudo apt-get update 
```

 Installing Docker's components:
 - docker-ce: Docker Engine, Docker's kernel
 - docker-ce-cli: Interface of Docker's command line
 - containerd.io: daemon containerd
 - docker-compose-plugin: Plugin Docker Compose
 ```bash
sudo apt-get install -y docker-ce docker-ce-cli containerd.io docker-compose-plugin
```


# 2. Download Docker image of Ubuntu 22 and run container

 Pull image Ubuntu 22.04
 ```bash
sudo docker pull ubuntu:22.04 
```

 Запись значения '0' в /proc/sys/kernel/yama/ptrace_scope
 In order to GDB could attach to procceses into Docker container
 ```bash
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope 
```

 Running Docker container by Ubuntu 22.04 image
 Flags:
 --cap-add=SYS_PTRACE: additional priviligies of ptrace (for debugging)
 -it: terminal mode
 --name my_ubuntu_container: container's name
 ```bash
sudo docker run \
	--cap-add=SYS_PTRACE \
	-it --name my_ubuntu_container ubuntu:22.04 /bin/bash
```
 (Optional) Starting container if it was stopped
 ```bash
 docker start my_ubuntu_container
 ```


# 3. Install packages for building

 Installing enviroment variable DEBIAN_FRONTEND in 'noninteractive'
 Omit all interactive queries for user
 ```bash
export DEBIAN_FRONTEND=noninteractive 
```
 Updating packages in Ubuntu container
 ```bash
apt-get update \

apt-get install -y \
	jq git tree nano vim gdb \
	build-essential cpanminus slapd ldap-utils libldap2-dev \
	autoconf bison clang-11 devscripts dpkg-dev flex \
	libldap2-dev libdbi-perl libgssapi-krb5-2 libicu-dev \
	krb5-kdc krb5-admin-server libssl-dev libpam0g-dev \
	libkrb5-dev krb5-user libcurl4-openssl-dev \
	perl perl-modules libipc-run-perl libtest-simple-perl libtime-hires-perl \
	liblz4-dev libpam-dev libreadline-dev libselinux1-dev libsystemd-dev \
	libxml2-dev libxslt-dev libzstd-dev llvm-11-dev locales-all pkg-config \
	python3-dev uuid-dev zlib1g-dev \
    openjade docbook-xml docbook-xsl opensp libxml2-utils xsltproc \
    libjson-perl curl time
```


# 4. Creating group and user in PostgreSQL


 Creating group 'postgres'
 ```bash
groupadd -r postgres 
```
 Creating user 'postgres' and adding him in 'postgres' group
 Options:
 -r: creating user
 -g postgres: poiner to group 
 -d /var/lib/postgresql: home directory
 -s /bin/bash: enviroment
```bash
useradd -r -g postgres -d /var/lib/postgresql -s /bin/bash postgres \
mkdir -p /var/lib/postgresql \
chown postgres:postgres /var/lib/postgresql \
chmod 700 /var/lib/postgresql \
mkdir -p /var/run/postgresql \
chown postgres:postgres /var/run/postgresql \
\
mkdir -p /var/log/postgresql \
chown postgres:postgres /var/log/postgresql

mkdir -p /usr/local/pgsql
chown postgres:postgres /usr/local/pgsql
```


# 5. Cloning and building PostgreSQL

 Switching into user 'postgres' and cloning PostgreSQL's source code
 switching into REL_16_STABLE branch and building PostgreSQL
 ```bash
su - postgres

git clone -b REL_16_STABLE https://github.com/postgres/postgres postgres_src
cd postgres_src/

./configure --prefix=/usr/local/pgsql \
	--enable-tap-tests \
	--with-python \
	--with-icu \
	--with-lz4 \
	--with-zstd \
	--enable-debug \
	--enable-cassert

make -j 4
make install
```
# 6. Initialization and launching PostgreSQL

 Creating directory for databases
 ```bash
mkdir /var/lib/postgresql/my_data
```
 Initialization cluster in created directory
 ```bash
/usr/local/pgsql/bin/initdb \
	-D /var/lib/postgresql/my_data
```
 Adding parameters in file of configuration postgresql.conffor for logging
 ```bash
cat >> /var/lib/postgresql/my_data/postgresql.conf << EOL
logging_collector = on
log_directory = '/var/log/postgresql'
log_statement = 'all'
log_min_messages = debug1
log_duration = on
log_filename = 'postgresql.log'
log_destination = 'csvlog'
log_error_verbosity = verbose
log_lock_waits = on
EOL
```
 Launching PostgreSQL with created directory
 ```bash
/usr/local/pgsql/bin/pg_ctl \
	-D /var/lib/postgresql/my_data \
	start
```

# 7. Work with PostgreSQL's extensions

 Creating a directory for the extension and writing the source
 code of the extension in C
 Here, an extension for PostgreSQL is created, which provides
 two functions:
 - calculate_sum_id: calculates a checksum for a given
       identifier
 - validate_check_digit: verifies the correctness of the check digit
       for a given identifier

 If you are in postgres_src directory, run compilation and installing
 ```bash
make -C contrib/my_first_extension/
make -C contrib/my_first_extension/ install
```
 Creating extension
 ```bash
/usr/local/pgsql/bin/psql -c 'create extension my_first_extension'
```
 Running examples
 ```bash
/usr/local/pgsql/bin/psql -c 'select generate_check_digit('345'::text)'
/usr/local/pgsql/bin/psql -c 'select generate_check_digit('59'::text)'
/usr/local/pgsql/bin/psql -c "select generate_check_digit('-1'::text)"
/usr/local/pgsql/bin/psql -c "select generate_check_digit('Hi'::text)"
/usr/local/pgsql/bin/psql -c 'select generate_check_digit('0'::text)'

/usr/local/pgsql/bin/psql -c 'select validate_check_digit('345'::text)'
/usr/local/pgsql/bin/psql -c 'select validate_check_digit('93'::text)'
/usr/local/pgsql/bin/psql -c 'select validate_check_digit('0'::text)'
/usr/local/pgsql/bin/psql -c "select validate_check_digit('Hi'::text)"
```
 Dropping extension
 ```bash
/usr/local/pgsql/bin/psql -c 'drop extension my_first_extension'
```
 Running regress tests
 ```bash
make -C contrib/my_first_extension/ check
```
 Cleaning of compilation
 ```bash
make -C contrib/my_first_extension/ clean
```


# 8. Work with PGXS

 Creating extension not in PostgreSQL's source code
 ```bash
cp -rf contrib/my_first_extension ../my_first_extension \
cd ../my_first_extension
```

 Building and running extension with PGXS
 ```bash
USE_PGXS=1 make
USE_PGXS=1 make install
USE_PGXS=1 make installcheck
```
 
 Creating extension
 ```bash
/usr/local/pgsql/bin/psql -c 'create extension my_first_extension'

/usr/local/pgsql/bin/psql -c 'select generate_check_digit('345'::text)'
/usr/local/pgsql/bin/psql -c 'select generate_check_digit('59'::text)'
/usr/local/pgsql/bin/psql -c "select generate_check_digit('-1'::text)"
/usr/local/pgsql/bin/psql -c "select generate_check_digit('Hi'::text)"
/usr/local/pgsql/bin/psql -c 'select generate_check_digit('0'::text)'

/usr/local/pgsql/bin/psql -c 'select validate_check_digit('345'::text)'
/usr/local/pgsql/bin/psql -c 'select validate_check_digit('93'::text)'
/usr/local/pgsql/bin/psql -c 'select validate_check_digit('0'::text)'
/usr/local/pgsql/bin/psql -c "select validate_check_digit('Hi'::text)"
```
# 9. Updating extension

 Adding new function
 ```bash
sed -i '/PG_FUNCTION_INFO_V1(generate_check_digit);/a PG_FUNCTION_INFO_V1(my_function);' \
	generate_check_digit.c \

echo 'Datum
my_function(PG_FUNCTION_ARGS)
{
       text  *name = PG_GETARG_TEXT_PP(0);
       uint64_t id = atol( name->vl_dat );

   return id % 10;
}' >> generate_check_digit.c
```
 Adding updating sql-script
 ```bash
cat > my_first_extension--1.0--1.1.sql << EOL
/* contrib/my_first_extension/my_first_extension--1.1.sql */
```
-- complain if script is sourced in psql, rather than via CREATE EXTENSION
```bash
\echo Use '''CREATE EXTENSION "my_first_extension"''' to load this file. \quit

CREATE OR REPLACE FUNCTION my_function(text)
RETURNS smallint
AS 'MODULE_PATHNAME', 'my_function'
IMMUTABLE STRICT LANGUAGE C PARALLEL SAFE;
EOL
```
 Adding sql-script in Makefile
 ```bash
sed -i 's/DATA = my_first_extension--1.0.sql/DATA = my_first_extension--1.0.sql my_first_extension--1.0--1.1.sql/' Makefile
```
 Changing default version in control file
 ```bash
sed -i 's/default_version = '\''1.0'\''/default_version = '\''1.1'\''/' my_first_extension.control
```
 Building and installing extension
 ```bash
USE_PGXS=1 make clean all
USE_PGXS=1 make install
```
 Updaing extension
 ```bash
/usr/local/pgsql/bin/psql -c 'alter extension my_first_extension update'
```
 Execute examples with new function
 ```bash
/usr/local/pgsql/bin/psql -c 'select my_function('30'::text)'
/usr/local/pgsql/bin/psql -c 'select my_function('0'::text)'
/usr/local/pgsql/bin/psql -c 'select my_function('345'::text)'
```
 Checking presence of old function
 ```bash
/usr/local/pgsql/bin/psql -c 'select generate_check_digit('345'::text)'
/usr/local/pgsql/bin/psql -c 'select generate_check_digit('59'::text)'
/usr/local/pgsql/bin/psql -c "select generate_check_digit('-1'::text)"
/usr/local/pgsql/bin/psql -c "select generate_check_digit('Hi'::text)"
/usr/local/pgsql/bin/psql -c 'select generate_check_digit('0'::text)'

/usr/local/pgsql/bin/psql -c 'select validate_check_digit('345'::text)'
/usr/local/pgsql/bin/psql -c 'select validate_check_digit('93'::text)'
/usr/local/pgsql/bin/psql -c 'select validate_check_digit('0'::text)'
/usr/local/pgsql/bin/psql -c "select validate_check_digit('Hi'::text)"
```
 Dropping extension
```bash
/usr/local/pgsql/bin/psql -c 'drop extension my_first_extension'
```