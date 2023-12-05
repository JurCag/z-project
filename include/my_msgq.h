#ifndef _MY_MSGQ_H
#define _MY_MSGQ_H

#define MSG_SIZE    16
#define MSGQ_SIZE   CONFIG_MSGQ_SIZE

int my_msgq_write_static(void);
char *my_msgq_read(void);

#endif