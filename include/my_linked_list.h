#ifndef _MY_LINKED_LIST_H
#define _MY_LINKED_LIST_H

#include <zephyr/shell/shell.h>

#define LINKED_LIST_DATA_ITEM_SIZE 16

typedef struct
{
    char data[LINKED_LIST_DATA_ITEM_SIZE];
} linked_list_item_data_t;

typedef struct
{
    sys_snode_t node;
    linked_list_item_data_t msg;
} linked_list_item_t;

// void append_data_to_linked_list(const struct shell *sh, size_t argc, char **argv);
void prepend_data_to_linked_list(const struct shell *sh, size_t argc, char **argv);
// void get_last_data_from_linked_list(const struct shell *sh, size_t argc, char **argv);
void get_first_data_from_linked_list(const struct shell *sh, size_t argc, char **argv);

int my_linked_list_alloc_and_append_data(const char *data);
linked_list_item_data_t* my_linked_list_get_last_data(void);
void my_linked_list_free_data(linked_list_item_data_t* data);



#endif