#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>

void trim(char *string);
int runCommand(char * commandString);


int main(int argc, char *argv[]) {
    int userInputLength = 512;
    char userInput[userInputLength];

    char *pipeCommand;
    char *userInputUpToRedir;
    char *redirOutputFileName;
    int redirOutputFD;
    int stdIn;
    int stdOut;
    int pipeIO[2];


    while (1) {
        // show shell prompt
        printf("am0165 > ");

        // save original stdin and stdout and restore after running commands
        stdIn = dup(0);
        stdOut = dup(1);

        // clear userInput buffer
        bzero(userInput, userInputLength);
        // get userInput
        fgets(userInput, userInputLength, stdin);

        // remove newline character at end
        userInput[strlen(userInput) - 1] = '\0';

        // split by '>', and save second split part to redirOutputFileName
        strtok(userInput, ">");
        redirOutputFileName = strtok(NULL, ">");

        // if there is a right side of '>' (we want to redirect output to a file)
        if (redirOutputFileName != NULL) {
            // trim the output file name, since there is a space after '>' character
            trim(redirOutputFileName);

            // open redirect output file descriptor
            redirOutputFD = open(redirOutputFileName, O_CREAT | O_RDWR | O_TRUNC, 0644);

            // replace stdout with redirOutputFD
            dup2(redirOutputFD, 1);
        }

        // split by '|', and save second split part to pipeCommand
        strtok(userInput, "|");
        pipeCommand = strtok(NULL, "|");

        // create a one way pipe, with two file descriptors stored in pipeIO
        // bytes written on PIPEDES[1] (WRITE end) can be read from PIPEDES[0] (READ end)
        if (pipe(pipeIO) == -1) {
            perror("Error with pipe()");
            goto ERROR;
        }

        if (pipeCommand != NULL) {
            trim(pipeCommand);

            pid_t PID = fork();
            if (PID == -1) {
                perror("Error with fork() for running userInput command");
                goto ERROR;
            } else if (PID == 0) {
                // child Process

                // replace stdout with pipe WRITE end
                dup2(pipeIO[1], 1);
                // close READ end in child process
                close(pipeIO[0]);

                // run userInput command, and the stdout will be piped through WRITE end
                runCommand((char*)userInput);

                exit(0);
            } else {
                // parent Process

                // replace stdin with pipe READ end
                dup2(pipeIO[0], 0);
                // close WRITE end in parent process
                close(pipeIO[1]);

                // wait for child process to run userInput command, and write stdout to pipe WRITE end, and exit
                wait(0);

                // run Pipe command
                runCommand(pipeCommand);
            }
        } else {
            // if no Pipe Command
            runCommand((char*)userInput);
        }

        dup2(stdIn, 0);
        dup2(stdOut, 1);
    }

    return 0;

    ERROR:
    return -1;
}

int runCommand(char * commandString) {
    char *args[32];
    // split pipeCommand by spaces
    args[0] = strtok(commandString, " ");
    int i = 0;
    while (args[i] != NULL) {
        i += 1;
        args[i] = strtok(NULL, " ");
    }


    pid_t PID = fork();
    if (PID == -1) {
        perror("Error with fork() for running pipe command");
        return -1;
    } else if (PID == 0) {
        // child process
        execvp(args[0], args);
    } else {
        // parent process

        // wait for child process to exit
        wait(0);
    }

    return 0;
}

void trim(char *string) {
    /* First remove leading spaces */

    const char *firstNonSpace = string;

    while (*firstNonSpace != '\0' && isspace(*firstNonSpace)) {
        ++firstNonSpace;
    }

    size_t len = strlen(firstNonSpace) + 1;

    memmove(string, firstNonSpace, len);

    /* Now remove trailing spaces */

    char *endOfString = string + len;

    while (string < endOfString && isspace(*endOfString)) {
        --endOfString;
    }

    *endOfString = '\0';

}