#include "postgres.h"
#include "lib/ilist.h"

#include "u_ilist.h"

typedef struct table {
    char* name;
    dlist_head rowHead;
} table;

typedef struct tableRow {
    int id;
    char* name;
    int mark;
    dlist_node node;
} tRow;

static tRow* create_row(table* tab, int id, char* name, int mark) {
    tRow* row = palloc(sizeof(tRow));
    row->id = id;
    row->name = name;
    row->mark = mark;
    dlist_push_tail(&tab->rowHead, &row->node);
    return row;
}

void executeIlist() {

    table* tab;
    tRow* row1;
    tRow* row2;
    tRow* row;
    dlist_iter iter;

    dlist_node* listNode;
    dlist_node* prev;

    tab = palloc(sizeof(table));
    tab->name = "table1";
    dlist_init(&tab->rowHead);

    row1 = create_row(tab, 1, "Hello", 70);
    row2 = create_row(tab, 2, "Hi", 77);

    listNode = &row1->node;
    row = dlist_container(tRow, node, listNode);
    elog(INFO, "%d %s %d", row->id, row->name, row->mark);

    listNode = &row2->node;
    prev = dlist_prev_node(&tab->rowHead, listNode);
    row = dlist_container(tRow, node, prev);
    elog(INFO, "%d %s %d", row->id, row->name, row->mark);

    row = dlist_head_element(tRow, node, &tab->rowHead);
    elog(INFO, "%d %s %d", row->id, row->name, row->mark);

    dlist_foreach(iter, &tab->rowHead) {
        row = dlist_container(tRow, node, iter.cur);
        elog(INFO, "%d %s %d", row->id, row->name, row->mark);
    }
}