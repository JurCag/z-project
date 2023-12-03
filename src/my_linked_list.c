#include "../include/my_linked_list.h"
#include <zephyr/kernel.h>
#include <string.h>

#define NECESSARY_LINKAGE_BYTES 4 // First 4 bytes of each unused block provide the necessary linkage.
#define MEM_BLOCKS_SIZE (LINKED_LIST_DATA_ITEM_SIZE + NECESSARY_LINKAGE_BYTES)
#define MEM_BLOCKS_AMOUNT 8
#define MEM_BUFFER_ALIGNMENT 4

K_MEM_SLAB_DEFINE(s, MEM_BLOCKS_SIZE, MEM_BLOCKS_AMOUNT, MEM_BUFFER_ALIGNMENT);
static sys_slist_t list = SYS_SLIST_STATIC_INIT(list);

int my_linked_list_alloc_and_append_data(const char *data)
{
    void *tmp = NULL;

    if (k_mem_slab_alloc(&s, &tmp, K_NO_WAIT) == 0)
    {
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
    return 0;
}

// linked_list_item_t *my_linked_list_get_last_data(void)
// {
//     linked_list_item_t *item = NULL;
//     sys_snode_t *tmp;
//     tmp = sys_slist_peek_tail(&list);

//     if (tmp != NULL)
//     {
//         item = CONTAINER_OF(tmp, linked_list_item_t, node);
//         return item;
//     }
//     else
//     {
//         return NULL;
//     }
// }

// void my_linked_list_free_data(linked_list_item_t *data)
// {
//     linked_list_item_t *item = NULL;
//     item = CONTAINER_OF(&data->node, linked_list_item_t, node);

//     sys_slist_find_and_remove(&list, &item->node);
//     k_mem_slab_free(&s, item);
// }
linked_list_item_data_t *my_linked_list_get_last_data(void)
{
    linked_list_item_t *item = NULL;
    sys_snode_t *tmp;
    tmp = sys_slist_peek_tail(&list);

    if (tmp != NULL)
    {
        item = CONTAINER_OF(tmp, linked_list_item_t, node);
        return &(item->msg);
    }
    else
    {
        return NULL;
    }
}

void my_linked_list_free_data(linked_list_item_data_t *data)
{
    linked_list_item_t *item = NULL;
    item = CONTAINER_OF(data, linked_list_item_t, msg);

    sys_slist_find_and_remove(&list, &item->node);
    k_mem_slab_free(&s, item);
}

// void append_data_to_linked_list(const struct shell *sh, size_t argc, char **argv)
// {
//     void *tmp = NULL;

//     if (k_mem_slab_alloc(&s, &tmp, K_NO_WAIT) == 0)
//     {
//         linked_list_item_t *elem = (linked_list_item_t *)tmp;
//         strncpy(elem->msg, "dynamic", sizeof(elem->msg));

//         sys_slist_append(&list, &elem->node);
//         shell_print(sh, "Appended '%s' at the end of the linked list", elem->msg);
//         return;
//     }
//     shell_print(sh, "Failed to allocate slab memory!");
// }

void prepend_data_to_linked_list(const struct shell *sh, size_t argc, char **argv)
{
    void *tmp = NULL;

    if (k_mem_slab_alloc(&s, &tmp, K_NO_WAIT) == 0)
    {
        linked_list_item_t *item = (linked_list_item_t *)tmp;
        strncpy(item->msg.data, "dynamic", sizeof(item->msg.data));

        sys_slist_prepend(&list, &item->node);
        shell_print(sh, "Prepended '%s' at the start of the linked list", item->msg.data);
        return;
    }
    shell_print(sh, "Failed to allocate slab memory!");
}

// void get_last_data_from_linked_list(const struct shell *sh, size_t argc, char **argv)
// {
//     linked_list_item_t *elem = NULL;
//     sys_snode_t *tmp;
//     tmp = sys_slist_peek_tail(&list);

//     if (tmp != NULL)
//     {
//         elem = CONTAINER_OF(tmp, linked_list_item_t, node);
//         shell_print(sh, "Received '%s' from linked list", elem->msg);
//         if (sys_slist_find_and_remove(&list, &elem->node) == true)
//         {
//             shell_print(sh, "Element removed");
//         }
//         k_mem_slab_free(&s, elem);
//     }
//     else
//     {
//         shell_print(sh, "Linked list is empty!");
//     }
// }

void get_first_data_from_linked_list(const struct shell *sh, size_t argc, char **argv)
{
    linked_list_item_t *item = NULL;
    sys_snode_t *tmp;
    tmp = sys_slist_peek_head(&list);

    if (tmp != NULL)
    {
        item = CONTAINER_OF(tmp, linked_list_item_t, node);
        shell_print(sh, "Received '%s' from linked list", item->msg.data);
        if (sys_slist_find_and_remove(&list, &item->node) == true)
        {
            shell_print(sh, "Element removed");
        }
        k_mem_slab_free(&s, item);
    }
    else
    {
        shell_print(sh, "Linked list is empty!");
    }
}