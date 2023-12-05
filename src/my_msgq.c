#include "../include/my_msgq.h"
#include <zephyr/kernel.h>

K_MSGQ_DEFINE(static_msgq, MSG_SIZE, MSGQ_SIZE, 1);

int my_msgq_write_static(void)
{
    const char message[] = "static";

    if (k_msgq_put(&static_msgq, message, K_NO_WAIT) == 0)
    {
        return 0;
    }
    return 1;
}

char *my_msgq_read(void)
{
    static char msg[MSG_SIZE];

    if (k_msgq_get(&static_msgq, msg, K_NO_WAIT) == 0)
    {
        return msg;
    }
    return NULL;
}
