#ifndef _MY_FIFO_H
#define _MY_FIFO_H

#include <stddef.h>

int my_fifo_alloc_and_push_data(char *data, size_t len);
char **my_fifo_get_data(void);
void my_fifo_free_data(char **data);

#endif