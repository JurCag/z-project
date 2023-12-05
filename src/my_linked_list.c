#include "my_linked_list.h"
#include <zephyr/kernel.h>
#include <string.h>

typedef struct
{
    sys_snode_t node;
    linked_list_item_data_t msg;
} linked_list_item_t;

#define NECESSARY_LINKAGE_BYTES 4 // First 4 bytes of each unused block provide the necessary linkage.
#define MEM_BLOCKS_SIZE (LINKED_LIST_DATA_ITEM_SIZE + NECESSARY_LINKAGE_BYTES)
#define MEM_BLOCKS_AMOUNT 8
#define MEM_BUFFER_ALIGNMENT 4

K_MEM_SLAB_DEFINE(s, MEM_BLOCKS_SIZE, MEM_BLOCKS_AMOUNT, MEM_BUFFER_ALIGNMENT);
static sys_slist_t list = SYS_SLIST_STATIC_INIT(list);

int my_linked_list_alloc_and_append_data(const char *data)
{
    void *tmp = NULL;

    if (k_mem_slab_alloc(&s, &tmp, K_NO_WAIT) != 0)
    {
        return 0;
    }

    linked_list_item_t *item = (linked_list_item_t *)tmp;
    if ((strlen(data) + 1) > LINKED_LIST_DATA_ITEM_SIZE)
    {
        strncpy(item->msg.data, data, LINKED_LIST_DATA_ITEM_SIZE - 1);
        item->msg.data[LINKED_LIST_DATA_ITEM_SIZE - 1] = '\0';
    }
    else
    {
        strncpy(item->msg.data, data, strlen(data));
        item->msg.data[strlen(data)] = '\0';
    }

    sys_slist_append(&list, &item->node);
    return strlen(item->msg.data);
}

linked_list_item_data_t *my_linked_list_get_last_data(void)
{
    linked_list_item_t *item = NULL;
    sys_snode_t *tmp;
    tmp = sys_slist_peek_tail(&list);

    if (tmp == NULL)
    {
        return NULL;
    }

    item = CONTAINER_OF(tmp, linked_list_item_t, node);
    return &(item->msg);
}

void my_linked_list_free_data(linked_list_item_data_t *data)
{
    linked_list_item_t *item = NULL;
    item = CONTAINER_OF(data, linked_list_item_t, msg);

    sys_slist_find_and_remove(&list, &item->node);
    k_mem_slab_free(&s, item);
}
