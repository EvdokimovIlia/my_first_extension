/* contrib/my_first_extension/my_first_extension--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use '''CREATE EXTENSION "validate_isr_id"''' to load this file. \quit
\echo Use '''CREATE EXTENSION "generate_check_digit"''' to load this file. \quit

CREATE FUNCTION validate_isr_id()
RETURNS boolean
AS 'MODULE_PATHNAME', 'validate_isr_id'
IMMUTABLE STRICT LANGUAGE C PARALLEL SAFE;

CREATE FUNCTION generate_check_digit()
RETURNS smallint
AS 'MODULE_PATHNAME', 'generate_check_digit'
IMMUTABLE STRICT LANGUAGE C PARALLEL SAFE;
