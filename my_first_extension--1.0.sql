/* contrib/my_first_extension/my_first_extension--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use '''CREATE EXTENSION "my_first_extension"''' to load this file. \quit

CREATE FUNCTION validate_check_digit(text)
RETURNS boolean
AS 'MODULE_PATHNAME', 'validate_check_digit'
IMMUTABLE STRICT LANGUAGE C PARALLEL SAFE;

CREATE FUNCTION generate_check_digit(text)
RETURNS smallint
AS 'MODULE_PATHNAME', 'generate_check_digit'
IMMUTABLE STRICT LANGUAGE C PARALLEL SAFE;
