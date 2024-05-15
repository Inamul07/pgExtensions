\echo USE "CREATE EXTENSION cache_ext" to load this file. \quit

CREATE FUNCTION get_row_from_table(id INTEGER, tableName TEXT) RETURNS SETOF bank_data
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;