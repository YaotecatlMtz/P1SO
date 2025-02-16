#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute_command(char *input) {
    int background = 0;
    char *args[20];
    int arg_count = 0;
    
    char *token = strtok(input, " ");
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            background = 1;
        } else {
            args[arg_count++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;
    
    if (arg_count == 0) return; 
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        if (!background) {
            waitpid(pid, NULL, 0);
        } else {
            printf("[Proceso en segundo plano: %d]\n", pid);
        }
    } else {
        perror("fork");
    }
}

int main() {
    char input[100];
    while (1) {
        printf("sh> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "exit") == 0) {
            exit(0);
        } else if (strcmp(input, "shutdown") == 0) {
            system("killall getty init sh");
            exit(0);
        } else {
            execute_command(input);
        }
    }
    return 0;
}

