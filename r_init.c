// init.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_GETTY 6

void start_getty() {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("xterm", "xterm", "-e", "./getty", NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }
}

int main() {
    pid_t pids[NUM_GETTY];
    for (int i = 0; i < NUM_GETTY; i++) {
        start_getty();
    }
    while (1) {
        int status;
        pid_t pid = wait(&status);
        if (pid > 0) {
            printf("Process %d terminated, restarting...\n", pid);
            start_getty();
        }
    }
    return 0;
}