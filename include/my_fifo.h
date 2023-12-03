#ifndef _MY_FIFO_H
#define _MY_FIFO_H

#include <stddef.h>

typedef struct {
    void *fifo_reserved;   /* 1st word reserved for use by FIFO */
    char *msg;
} fifo_item_t;

int my_fifo_alloc_and_push_data(char *data, size_t len);
char **my_fifo_get_data(void);
void my_fifo_free_data(char **data);

// void my_fifo_push_data(const struct shell *sh, size_t argc, char **argv);
// void my_fifo_get_data(const struct shell *sh, size_t argc, char **argv);

// fifo_item_t *my_fifo_get_data(void);
// void my_fifo_free_data(fifo_item_t *data);




#endif