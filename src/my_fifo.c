#include "../include/my_fifo.h"
#include <zephyr/kernel.h>
#include <string.h>

typedef struct
{
    void *fifo_reserved; /* 1st word reserved for use by FIFO */
    char *msg;
} fifo_item_t;

K_FIFO_DEFINE(dynamic_fifo);
K_HEAP_DEFINE(h, 256);

int my_fifo_alloc_and_push_data(char *data, size_t len)
{
    static fifo_item_t *tx_data_item = NULL;

    tx_data_item = k_heap_alloc(&h, sizeof(fifo_item_t), K_NO_WAIT); // alloc memory for data item
    if(tx_data_item == NULL)
    {
        return 1;
    }
    
    tx_data_item->msg = NULL;
    tx_data_item->msg = k_heap_alloc(&h, len + 1, K_NO_WAIT); // alloc memory for message
    if (tx_data_item->msg == NULL)
    {
        return 1;
    }

    tx_data_item->msg[len] = '\0';
    strncpy(tx_data_item->msg, data, len);
    k_fifo_put(&dynamic_fifo, tx_data_item);
}

char **my_fifo_get_data(void)
{
    static fifo_item_t *rx_data_item = NULL;
    rx_data_item = k_fifo_get(&dynamic_fifo, K_NO_WAIT);

    if (rx_data_item != NULL)
    {
        return &(rx_data_item->msg);
    }
    return NULL;
}

void my_fifo_free_data(char **data)
{
    fifo_item_t *item = NULL;
    item = CONTAINER_OF(data, fifo_item_t, msg);

    if (item != NULL)
    {
        k_heap_free(&h, item->msg);
        k_heap_free(&h, item);
    }
}
