#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

const char *processes_to_kill[] = {"sh", "getty", "init"};
const int num_processes = 3;

void execute_command(char *command) {
    if (strcmp(command, "exit") == 0) {
        exit(0);
    } 
    else if (strcmp(command, "shutdown") == 0) {
        printf("System shutting down...\n");

        for (int i = 0; i < num_processes; i++) {
            pid_t pid = fork();
            if (pid == 0) {
                execlp("pkill", "pkill", processes_to_kill[i], NULL);
                perror("Error ejecutando pkill");
                exit(EXIT_FAILURE);
            } else if (pid > 0) {
                waitpid(pid, NULL, 0);
            } else {
                perror("Error en fork");
            }
        }

        exit(0);
    } 
    else {
        pid_t pid = fork();
        if (pid == 0) {  
            char *args[] = {command, NULL}; 
            execvp(command, args);
            perror("Error ejecutando comando");
            exit(EXIT_FAILURE);
        } 
        else if (pid > 0) {  
            waitpid(pid, NULL, 0);
        } 
        else {  
            perror("Error en fork");
        }
    }
}

int main() {
    char command[100];

    while (1) {
        printf("sh> ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }
        
        command[strcspn(command, "\n")] = 0;
        
        execute_command(command);
    }

    return 0;
}

    return 0;
}
