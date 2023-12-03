#ifndef _MY_MSGQ_H
#define _MY_MSGQ_H

#include <stddef.h>
#include <zephyr/shell/shell.h>

#define MSG_SIZE    16
#define MSGQ_SIZE   CONFIG_MSGQ_SIZE

extern struct k_msgq static_msgq;

int my_msgq_write_static(const struct shell *sh, size_t argc, char **argv);
int my_msgq_read(const struct shell *sh, size_t argc, char **argv);

#endif