/*
 * Copyright (c) 2015 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/shell/shell.h>
#include <stdlib.h>

#include "../include/my_led.h"
#include "../include/my_btn.h"
#include "../include/my_msgq.h"
#include "../include/my_fifo.h"
#include "../include/my_linked_list.h"

static int led_on(const struct shell *sh, size_t argc, char **argv);
static int led_off(const struct shell *sh, size_t argc, char **argv);
static int led_toggle(const struct shell *sh, size_t argc, char **argv);

static int write_static_to_msgq(const struct shell *sh, size_t argc, char **argv);
static int read_from_msgq(const struct shell *sh, size_t argc, char **argv);

static int push_data_to_fifo(const struct shell *sh, size_t argc, char **argv);
static int get_data_from_fifo(const struct shell *sh, size_t argc, char **argv);

static int append_data_to_linked_list(const struct shell *sh, size_t argc, char **argv);
static int get_last_data_from_linked_list(const struct shell *sh, size_t argc, char **argv);

LOG_MODULE_REGISTER(app);

static int led_on(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    my_led_on();
    shell_print(sh, "Led ON");
    return 0;
}

static int led_off(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    my_led_off();
    shell_print(sh, "Led OFF");
    return 0;
}

static int led_toggle(const struct shell *sh, size_t argc, char **argv)
{
    static int period = 1000;
    if (argc > 1)
    {
        period = atoi(argv[1]);
        if (period <= 0)
        {
            shell_print(sh, "Invalid period!");
            return -EINVAL;
        }
    }
    my_led_toggle(period);
    shell_print(sh, "Flashing LED with period %d ms", period);
    return 0;
}

static int write_static_to_msgq(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    if (my_msgq_write_static() == 0)
    {
        shell_print(sh, "Message written to MSGQ");
        return 0;
    }

    shell_print(sh, "MSGQ is full!");
    return 1;
}

static int read_from_msgq(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    static char *msg;

    msg = my_msgq_read();

    if (msg != NULL)
    {
        shell_print(sh, "Received from MSGQ: '%s'", msg);
        return 0;
    }

    shell_print(sh, "MSGQ empty!");
    return 1;
}

static int push_data_to_fifo(const struct shell *sh, size_t argc, char **argv)
{
    static int ret;
    static char *input_arg;

    if (argc > 1)
    {
        ret = my_fifo_alloc_and_push_data(argv[1], strlen(argv[1]));

        if (ret == 0)
        {
            shell_print(sh, "Pushed '%s' into FIFO", argv[1]);
            return 0;
        }

        shell_print(sh, "Failed to allocate memory!");
        return 1;
    }

    shell_print(sh, "Missing input argument!");
    return 1;
}

static int get_data_from_fifo(const struct shell *sh, size_t argc, char **argv)
{
    char **data = NULL;
    data = my_fifo_get_data();

    if (data != NULL)
    {
        shell_print(sh, "Received '%s' from FIFO", *data);
        my_fifo_free_data(data);
        return 0;
    }
    else
    {
        shell_print(sh, "FIFO is empty!");
        return 1;
    }
}

static int append_data_to_linked_list(const struct shell *sh, size_t argc, char **argv)
{
    static char msg[] = "dynamic";
    int size_appended = my_linked_list_alloc_and_append_data(msg);

    if (size_appended == strlen(msg))
    {
        shell_print(sh, "Appended '%s' at the end of the linked list", msg);
        return 0;
    }
    else if (size_appended > 0)
    {
        msg[size_appended] = '\0';
        shell_print(sh, "Appended '%s' (truncated) at the end of the linked list", msg);
        return 0;
    }

    shell_print(sh, "Failed to allocate slab memory!");
    return 1;
}

static int get_last_data_from_linked_list(const struct shell *sh, size_t argc, char **argv)
{
    linked_list_item_data_t *item = NULL;
    item = my_linked_list_get_last_data();

    if (item != NULL)
    {
        shell_print(sh, "Received '%s' from linked list", item->data);
        my_linked_list_free_data(item);
        return 0;
    }
    else
    {
        shell_print(sh, "Linked list is empty!");
        return 1;
    }
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    led_subcmds,
    SHELL_CMD_ARG(on, NULL, "Turn LED on.", &led_on, 1, 0),
    SHELL_CMD_ARG(off, NULL, "Turn LED off.", &led_off, 1, 0),
    SHELL_CMD_ARG(toggle, NULL, "Toggle LED with given period (ms).", &led_toggle, 1, 1),
    SHELL_SUBCMD_SET_END);
SHELL_CMD_REGISTER(led, &led_subcmds, "Led control commands", NULL);

SHELL_STATIC_SUBCMD_SET_CREATE(
    msgq_subcmds,
    SHELL_CMD_ARG(write, NULL, "Write 'static' to MSGQ", write_static_to_msgq, 1, 0),
    SHELL_CMD_ARG(read, NULL, "Pick up and dump message from MSGQ", read_from_msgq, 1, 0),
    SHELL_SUBCMD_SET_END);
SHELL_CMD_REGISTER(msgq, &msgq_subcmds, "Message queue commands", NULL);

SHELL_STATIC_SUBCMD_SET_CREATE(
    fifo_subcmds,
    SHELL_CMD_ARG(push, NULL, "Write given message to FIFO", push_data_to_fifo, 1, 1),
    SHELL_CMD_ARG(read, NULL, "Pick up and dump message from FIFO", get_data_from_fifo, 1, 0),
    SHELL_SUBCMD_SET_END);
SHELL_CMD_REGISTER(fifo, &fifo_subcmds, "FIFO commands", NULL);

SHELL_STATIC_SUBCMD_SET_CREATE(
    linked_list_subcmds,
    SHELL_CMD_ARG(append, NULL, "Append message at the end of linked list", append_data_to_linked_list, 1, 0),
    SHELL_CMD_ARG(read_last, NULL, "Read last message from linked list", get_last_data_from_linked_list, 1, 0),
    SHELL_SUBCMD_SET_END);
SHELL_CMD_REGISTER(linked_list, &linked_list_subcmds, "Linked list commands", NULL);

int main(void)
{
    int ret;

    ret = my_led_init();
    if (ret != 0)
    {
        return 0;
    }

    ret = my_btn_init();
    if (ret != 0)
    {
        return 0;
    }

    return 0;
}
