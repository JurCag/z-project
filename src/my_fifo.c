#include "../include/my_fifo.h"
#include <zephyr/kernel.h>
#include <string.h>

K_FIFO_DEFINE(dynamic_fifo);
K_HEAP_DEFINE(h, 256);

int my_fifo_alloc_and_push_data(char *data, size_t len)
{
    static fifo_item_t *tx_data_item = NULL;

    tx_data_item = k_heap_alloc(&h, sizeof(fifo_item_t), K_NO_WAIT); // alloc memory for data item
    if (tx_data_item != NULL)
    {
        tx_data_item->msg = NULL;
        tx_data_item->msg = k_heap_alloc(&h, len + 1, K_NO_WAIT);   // alloc memory for message

        if (tx_data_item->msg != NULL)
        {
            tx_data_item->msg[len] = '\0';

            strncpy(tx_data_item->msg, data, len);
            k_fifo_put(&dynamic_fifo, tx_data_item);
            return 0;
        }
    }
    return 1;
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

// fifo_item_t *my_fifo_get_data(void)
// {
//     static fifo_item_t *rx_data_item = NULL;
//     rx_data_item = k_fifo_get(&dynamic_fifo, K_NO_WAIT);
//     if (rx_data_item != NULL)
//     {
//         return rx_data_item;
//     }
//     return NULL;
// }

// void my_fifo_free_data(fifo_item_t *data)
// {
//     if (data != NULL)
//     {
//         k_heap_free(&h, data->msg);
//         k_heap_free(&h, data);
//     }
// }

// void my_fifo_push_data(const struct shell *sh, size_t argc, char **argv)
// {
//     static fifo_item_t *tx_data;
//     static char *input_arg;

//     if (argc > 1)
//     {
//         input_arg = argv[1];
//         tx_data = k_heap_alloc(&h, sizeof(fifo_item_t), K_NO_WAIT);

//         if (tx_data != NULL)
//         {
//             tx_data->msg = NULL;
//             tx_data->msg = k_heap_alloc(&h, strlen(input_arg) + 1, K_NO_WAIT);

//             if (tx_data->msg != NULL)
//             {
//                 tx_data->msg[strlen(input_arg)] = '\0';

//                 strncpy(tx_data->msg, input_arg, strlen(input_arg));
//                 k_fifo_put(&dynamic_fifo, tx_data);

//                 shell_print(sh, "Pushed '%s' into FIFO", tx_data->msg);
//                 return;
//             }
//         }
//         shell_print(sh, "Failed to allocate memory!");
//     }

//     shell_print(sh, "Missing input argument!");
// }

// void my_fifo_get_data(const struct shell *sh, size_t argc, char **argv)
// {
//     static fifo_item_t *rx_data;
//     rx_data = k_fifo_get(&dynamic_fifo, K_NO_WAIT);

//     if (rx_data != NULL)
//     {
//         shell_print(sh, "Received '%s' from FIFO", rx_data->msg);
//         k_heap_free(&h, rx_data->msg);
//         k_heap_free(&h, rx_data);
//     }
//     else
//     {
//         shell_print(sh, "FIFO is empty!");
//     }
// }
