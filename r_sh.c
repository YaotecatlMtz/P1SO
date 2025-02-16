// sh.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute_command(char *command) {
    int background = 0;
    size_t len = strlen(command);
    if (len > 0 && command[len - 1] == '&') {
        background = 1;
        command[len - 1] = '\0';
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        execlp(command, command, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        if (!background) {
            waitpid(pid, NULL, 0);
        }
    } else {
        perror("fork");
    }
}

int main() {
    char command[100];
    while (1) {
        printf("sh> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;
        
        if (strcmp(command, "exit") == 0) {
            exit(0);
        } else if (strcmp(command, "shutdown") == 0) {
            system("killall getty init sh");
            exit(0);
        } else {
            execute_command(command);
        }
    }
    return 0;
}
