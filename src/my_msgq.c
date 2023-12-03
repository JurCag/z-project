#include "../include/my_msgq.h"
#include <zephyr/kernel.h>

K_MSGQ_DEFINE(static_msgq, MSG_SIZE, MSGQ_SIZE, 1);

SHELL_STATIC_SUBCMD_SET_CREATE(
    msgq_subcmds,
    SHELL_CMD_ARG(write, NULL, "Write 'static' to MSGQ", my_msgq_write_static, 1, 0),
    SHELL_CMD_ARG(read, NULL, "Pick up and dump message from MSGQ", my_msgq_read, 1, 0),
    SHELL_SUBCMD_SET_END);
SHELL_CMD_REGISTER(msgq, &msgq_subcmds, "Message queue commands", NULL);

int my_msgq_write_static(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    const char message[] = "static";

    if (k_msgq_put(&static_msgq, message, K_NO_WAIT) == 0)
    {
        shell_print(sh, "Message 'static' written to MSGQ");
    }
    else
    {
        shell_print(sh, "MSGQ is full!");
    }
    return 0;
}

int my_msgq_read(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    static char msg[MSG_SIZE];

    if (k_msgq_get(&static_msgq, msg, K_NO_WAIT) == 0)
    {
        shell_print(sh, "Received from MSGQ: '%s'", msg);
    }
    else
    {
        shell_print(sh, "MSGQ empty!");
    }
    return 0;
}
