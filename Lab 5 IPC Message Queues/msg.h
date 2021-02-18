#ifndef MSG_H
#define MSG_H

#include <sys/types.h>

#define MSG_DATA_LENGTH 100


struct msg
{
    __syscall_slong_t mtype;	/* type of received/sent message */
    char mtext[MSG_DATA_LENGTH];		/* text of the message */
};

key_t MSG_QUEUE_KEY = 12345;

#endif