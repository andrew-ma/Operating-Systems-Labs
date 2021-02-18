#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h> 

#include <string.h>
#include <signal.h>

#include "msg.h"

void INThandler(int);
int msid;

int main(void) {
    signal(SIGINT, INThandler);

    // processB looks for existing message queue
    msid = msgget(MSG_QUEUE_KEY, 0);
    if (msid == -1) {
        // error getting message queue
        fprintf(stderr, "Failed to get existing message queue\n");
        exit(1);
    }

    // we don't have to worry about synchronization since message queues handle that in the msgsnd and msgrcv functions

    // set processB's to send type 2, and receive type 1
    long sendMsgType = 2;
    long receiveMsgType = 1;

    struct msg sendMsg;
    struct msg receiveMsg;
    // initialize defaults (will be using sendMsg and receiveMsg buffers so mtype will persist until changed manually)
    sendMsg.mtype = sendMsgType;
    receiveMsg.mtype = receiveMsgType;


    while (1) {
        printf("Waiting ...\n");
        fflush(stdout);
        // flag is 0, so it will wait until it receives a message
        if (msgrcv(msid, &receiveMsg, MSG_DATA_LENGTH, receiveMsgType, 0) == -1) {
            // error
            fprintf(stderr, "Failed to receive message: type %ld\n", receiveMsg.mtype);
            exit(1);
        }
        else {
            // read successful

            // if received message text is "Exit", then exit processB
            if (strncmp(receiveMsg.mtext, "Exit", strlen(receiveMsg.mtext)) == 0) {
                break;
            }

            printf("A: '%s'\n", receiveMsg.mtext);
            fflush(stdout);
        }

        printf("B > ");
        fflush(stdout);
        // get user input and save to sendMsg
        fgets(sendMsg.mtext, MSG_DATA_LENGTH, stdin);
        // remove \n character than fgets keeps
        (sendMsg.mtext)[strlen(sendMsg.mtext) - 1] = '\0';

        // send message to Queue
        if (msgsnd(msid, &sendMsg, MSG_DATA_LENGTH, IPC_NOWAIT) == -1) {
            // error
            fprintf(stderr, "Failed to send message: type %ld, text '%s'\n", sendMsg.mtype, sendMsg.mtext);
            exit(1);
        }
        else {
            // send successful

            // if processB sent "Exit" to processA, exit processB
            if (strncmp(sendMsg.mtext, "Exit", strlen(sendMsg.mtext)) == 0) {
                break;
            }
        }
    }

    exit(0);
}


void INThandler(int sig) {
    exit(0);
}
