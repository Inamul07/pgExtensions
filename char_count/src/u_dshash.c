#include "postgres.h"
#include "lib/dshash.h"

#include "u_dshash.h"

typedef struct tableRow {
    int id;
    char* name;
    int mark;
} tRow;

void executeHashMap() {

    dsa_area* area = dsa_create(-1);
    dshash_parameters* params = palloc(sizeof(dshash_parameters));
    dshash_table* table;
    tRow* row;
    bool* found = palloc(sizeof(bool));
    tRow* hvalue;
    tRow* search;
    bool val;

    params->key_size = sizeof(int);
    params->tranche_id = -1;
    params->entry_size = sizeof(tRow*);
    params->compare_function = dshash_memcmp;
    params->hash_function = dshash_memhash;
    table = dshash_create(area, params, NULL);

    row = palloc(sizeof(tRow));
    row->id = 1;
    row->name = "Hello";
    row->mark = 100;

    hvalue = (tRow*) dshash_find_or_insert(table, &(row->id), found);
    if(!(*found)) {
        memcpy(hvalue, row, sizeof(tRow));
        elog(INFO, "NOT FOUND");
    } else {
        elog(INFO, "FOUND");
    }
    dshash_release_lock(table, hvalue);

    row->id = 2;

    val = (tRow*) dshash_delete_key(table, &(row->id));
    if(val) {
        elog(INFO, "FOUND & DELETED");
    } else {
        elog(INFO, "NOT FOUND");
    }

    row->id = 1;

    search = (tRow*) dshash_find(table, &(row->id), false);
    if(search) {
        elog(INFO, "%d %s %d", search->id, search->name, search->mark);
        dshash_release_lock(table, search);
    } else {
        elog(INFO, "NOT FOUND");
    }

    dsa_dump(area);
    dsa_detach(area);
    dshash_detach(table);
    pfree(row);
    pfree(params);
}