#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void execute_command(char *command) {
    if (strcmp(command, "exit") == 0) {
        exit(0);
    } else if (strcmp(command, "shutdown") == 0) {
        pid_t pid = fork();
        if (pid == 0) {
            execlp("shutdown", "shutdown", "-h", "now", NULL);
            perror("shutdown failed");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            waitpid(pid, NULL, 0);
        } else {
            perror("fork");
        }
    } else {
        pid_t pid = fork();
        if (pid == 0) {
            execlp(command, command, NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            waitpid(pid, NULL, 0);
        } else {
            perror("fork");
        }
    }
}

int main() {
    char command[100];

    while (1) {
        printf("sh> ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break; // Handle EOF (Ctrl+D)
        }
        
        command[strcspn(command, "\n")] = 0; // Remove newline character
        execute_command(command);
    }

    return 0;
}
