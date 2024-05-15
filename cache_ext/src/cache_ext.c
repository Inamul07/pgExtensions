#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

PG_MODULE_MAGIC;

#include "libpq-fe.h"
#include "access/htup.h"
#include "access/htup_details.h"
#include "funcapi.h"
#include "access/tupdesc.h"
#include "miscadmin.h"
#include "lib/ilist.h"
#include "storage/shmem.h"
#include "utils/hsearch.h"
#include "storage/ipc.h"

typedef struct tableRow {
    int id;
    char name[NAMEDATALEN];
    int balance;
} tRow;

static shmem_request_hook_type prev_shmem_request_hook = NULL;
static shmem_startup_hook_type prev_shmem_startup_hook = NULL;
static HTAB* hashTable = NULL;
static dlist_head* headNode = NULL;
static void shmem_request(void);
static void shmem_startup(void);
Datum struct_row_get_datum(tRow* row, char* tableName);
tRow* get_row_from_db(int32 id, char* tableName);

// Getting Shared Memory
void _PG_init(void) {
    if(!process_shared_preload_libraries_in_progress) {
        return;
    }

    prev_shmem_request_hook = shmem_request_hook;
	shmem_request_hook = shmem_request;
	prev_shmem_startup_hook = shmem_startup_hook;
	shmem_startup_hook = shmem_startup;
    elog(INFO, "INIT");
}

static Size memsize(void) {
    Size size;

    size = MAXALIGN(sizeof(tRow));
    size = add_size(size, hash_estimate_size(5000, sizeof(tRow)));

    return size;
}

static void shmem_request(void) {
	if (prev_shmem_request_hook)
		prev_shmem_request_hook();

	RequestAddinShmemSpace(memsize());
}

static void shmem_startup(void) {
    bool headNodeFound;
    HASHCTL info;

    if(prev_shmem_request_hook) {
        prev_shmem_request_hook();
    }

    headNode = ShmemInitStruct("list_head_node", sizeof(dlist_head), &headNodeFound);

    if(!headNodeFound) {
        dlist_init(headNode);
    }

    info.keysize = NAMEDATALEN;
    info.entrysize = sizeof(tRow);
    hashTable = ShmemInitHash("cache_hash_table", 5000, 5000, &info, HASH_ELEM | HASH_STRINGS);
    elog(INFO, "SUCCESS %p", hashTable);
}
//

Datum struct_row_get_datum(tRow* row, char* tableName) {
    Datum values[3];
    bool nulls[3];
    HeapTuple tuple;
    TupleDesc desc = RelationNameGetTupleDesc(tableName);

    values[0] = Int32GetDatum(row->id);
    values[1] = CStringGetDatum(row->name);
    values[2] = Int32GetDatum(row->balance);

    nulls[0] = nulls[1] = nulls[2] = false;
    tuple = heap_form_tuple(desc, values, nulls);

    return HeapTupleGetDatum(tuple);
}

tRow* get_row_from_db(int32 id, char* tableName) {
    PGconn* conn = PQconnectdb("host=localhost port=5432 dbname=learn");
    PGresult* res;
    char* query;
    tRow* row;
    

    if (PQstatus(conn) != CONNECTION_OK) {
        PQfinish(conn);
        elog(ERROR, "Failed to connect to the database");
    }

    query = psprintf("SELECT * FROM %s WHERE account_number = %d", tableName, id);
    res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        PQfinish(conn);
        elog(ERROR, "Query execution failed");
    }

    if (PQntuples(res) == 0) {
        PQclear(res);
        PQfinish(conn);
        elog(ERROR, "No rows found");
    }

    row = malloc(sizeof(tRow));
    row->id = atoi(PQgetvalue(res,0, 0));
    strcpy(row->name, PQgetvalue(res, 0, 1));
    row->balance = atoi(PQgetvalue(res, 0, 2));

    PQclear(res);
    PQfinish(conn);
    
    return row;
}


PG_FUNCTION_INFO_V1(get_row_from_table);
Datum get_row_from_table(PG_FUNCTION_ARGS) {
    int32 id = PG_GETARG_INT32(0);
    text* textStr = PG_GETARG_TEXT_PP(1);
    char* tableName = text_to_cstring(textStr);
    bool idFound;
    tRow* row;
    tRow* newRow;

    // Check if id is in cache
    row = hash_search(hashTable, &id, HASH_FIND, &idFound);

    // Return if it is present
    if(idFound) {
        elog(INFO, "{ FOUND }");
        PG_RETURN_DATUM(struct_row_get_datum(row, tableName));
    } 

    elog(INFO, "{ NOT FOUND }");

    // TODO Add to Cache
    // return it from db
    row = get_row_from_db(id, tableName);

    newRow = hash_search(hashTable, &id, HASH_ENTER, NULL);
    newRow->id = row->id;
    strcpy(newRow->name, row->name);
    newRow->balance = row->balance;

    elog(INFO, "%ld", hash_get_num_entries(hashTable));

    PG_RETURN_DATUM(struct_row_get_datum(newRow, tableName));
}