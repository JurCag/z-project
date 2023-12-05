#ifndef _MY_LINKED_LIST_H
#define _MY_LINKED_LIST_H

#define LINKED_LIST_DATA_ITEM_SIZE 16

typedef struct
{
    char data[LINKED_LIST_DATA_ITEM_SIZE];
} linked_list_item_data_t;

int my_linked_list_alloc_and_append_data(const char *data);
linked_list_item_data_t *my_linked_list_get_last_data(void);
void my_linked_list_free_data(linked_list_item_data_t *data);

#endif