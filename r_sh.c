#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void execute_command(char *command) {
    if (strcmp(command, "exit") == 0) {
        exit(0);
    } else if (strcmp(command, "shutdown") == 0) {
        system("killall getty init sh");
        exit(0);
    } else {
        pid_t pid = fork();
        if (pid == 0) {
            execlp(command, command, NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        } else {
            wait(NULL);
        }
    }
}

int main() {
    char command[100];
    while (1) {
        printf("sh> ");
        scanf("%s", command);
        execute_command(command);
    }
    return 0;
    return 0;
}
