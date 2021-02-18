#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h> 

#include <string.h>
#include <signal.h>

#include "msg.h"

void INThandler(int);
int msid;

void closeMessageQueue(int msqid) {
    // close message queue
    if (msgctl(msid, IPC_RMID, NULL) == -1) {
        // error closing message queue
        fprintf(stderr, "Failed to close message queue\n");
    }
    else {
        printf("Closed Message Queue Successfully\n");
        fflush(stdout);
    }
}

int main(void) {
    signal(SIGINT, INThandler);

    // processA creates a new message queue with permissions rw-rw-rw
    msid = msgget(MSG_QUEUE_KEY, IPC_CREAT | 0666);
    if (msid == -1) {
        // error creating new message queue
        fprintf(stderr, "Failed to create new message queue\n");
        exit(1);
    }

    // we don't have to worry about synchronization since message queues handle that in the msgsnd and msgrcv functions

    // set processA's to send type 1, and receive type 2
    long sendMsgType = 1;
    long receiveMsgType = 2;

    struct msg sendMsg;
    struct msg receiveMsg;
    // initialize defaults (will be using sendMsg and receiveMsg buffers so mtype will persist until changed manually)
    sendMsg.mtype = sendMsgType;
    receiveMsg.mtype = receiveMsgType;

    while (1) {
        printf("A > ");
        fflush(stdout);
        // get user input and save to sendMsg
        fgets(sendMsg.mtext, MSG_DATA_LENGTH, stdin);
        // remove \n character than fgets keeps
        sendMsg.mtext[strlen(sendMsg.mtext) - 1] = '\0';

        // send message to Queue
        if (msgsnd(msid, &sendMsg, MSG_DATA_LENGTH, IPC_NOWAIT) == -1) {
            // error
            fprintf(stderr, "Failed to send message: type %ld, text '%s'\n", sendMsg.mtype, sendMsg.mtext);
            closeMessageQueue(msid);
            exit(1);
        }
        else {
            // sent successful
            // If processA sent "Exit" to processB, then break
            if (strncmp(sendMsg.mtext, "Exit", strlen(sendMsg.mtext)) == 0) {
                break;
            }
        }

        printf("Waiting ...\n");
        fflush(stdout);
        // flag is 0, so it will wait until it receives a message
        if (msgrcv(msid, &receiveMsg, MSG_DATA_LENGTH, receiveMsgType, 0) == -1) {
            // error
            fprintf(stderr, "Failed to receive message: type %ld\n", receiveMsg.mtype);
            closeMessageQueue(msid);
            exit(1);
        }
        else {
            // receive successful

            // if B sent "Exit" to A, exit A
            if (strncmp(receiveMsg.mtext, "Exit", strlen(receiveMsg.mtext)) == 0) {
                break;
            }

            printf("B: '%s'\n", receiveMsg.mtext);
            fflush(stdout);
        }
    }


    closeMessageQueue(msid);
    exit(0);
}


void INThandler(int sig) {
    closeMessageQueue(msid);
    exit(0);
}
