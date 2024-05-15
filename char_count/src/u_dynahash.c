#include "postgres.h"   
#include "u_dynahash.h"

#include "utils/dynahash.h"
#include "utils/hsearch.h"
#include "pg_config_manual.h" // NAMEDATALEN

typedef struct tableRow {
    int id;
    char name[NAMEDATALEN];
    int mark;
} tRow;

void executeDynahash() {
    HASHCTL ctl;
    HTAB* table;
    tRow* row;
    tRow* tempRow;
    bool valFound;
    char* name = "Hello";
    
    ctl.keysize = NAMEDATALEN;
    ctl.entrysize = sizeof(tRow);

    table = hash_create("Table HashMap", 16, &ctl, HASH_ELEM | HASH_STRINGS);

    row = palloc(sizeof(tRow));
    row->id = 1;
    strcpy(row->name, name);
    row->mark = 100;
    elog(INFO, "%d %d %s", row->id, row->mark, row->name);

    tempRow = hash_search(table, &(row->id), HASH_ENTER, &valFound);
    tempRow->id = row->id;
    tempRow->mark = row->mark;
    strcpy(tempRow->name, row->name);
    elog(INFO, "%d %d %s", tempRow->id, tempRow->mark, tempRow->name);

    tempRow = hash_search(table, &(row->id), HASH_FIND, &valFound);
    if(!valFound) {
        elog(INFO, "Value Not Found");
    } else {
        elog(INFO, "%d %d %s", tempRow->id, tempRow->mark, tempRow->name);
    }
}