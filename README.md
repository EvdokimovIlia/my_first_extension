# PostgreSQL_Add_Extention

Example of creating extension with function for PostgreSQL.

If PostgreSQL is NOT installed:
1. Clone this repository into PostgreSQL repository in contrib/ before compiling PostgreSQL.
  git clone .... <path_to_postgresql>/postgresql/contrib/
2. Add in contrib/Makefile and contrib/Makefile_se the string with new directory to new extension
3. Build PostgreSQL: make && sudo make install
4. Log in postgresql and execute the command: CREATE EXTENSION "israel".

If PostgreSQL is installed
1. Clone this repository into PostgreSQL repository in contrib/ before compiling PostgreSQL.
  git clone .... <path_to_postgresql>/postgresql/contrib/
2. Compile this directory
3. Stop PostgreSQL if it's running: sudo systemctl stop postgresql.service
4. Add sql-file into postgresql/extension/ and copy so-file into lib/postgresql/
5. Start PostgreSQL: sudo systemctl start postgresql.service
6. Log in postgresql and execute the command: CREATE EXTENSION "israel".
