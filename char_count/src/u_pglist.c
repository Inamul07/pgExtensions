#include "postgres.h"
#include "nodes/pg_list.h"

#include "u_pglist.h"

typedef struct tableRow {
    int id;
    char* name;
    int mark;
} tRow;

void executePgList() {
    ListCell* head = NULL;
    List* list = list_make1(head);
    ListCell* listCell = NULL;

    list = lappend(list, &(tRow) {.id=10, .name="Hi", .mark=77});
    foreach(listCell, list) {
        tRow* row = (tRow*) listCell->ptr_value;
        if(row) {
            elog(INFO, "%d %s %d", row->id, row->name, row->mark);
            printf("%d %s %d", row->id, row->name, row->mark);
        }
    }
}