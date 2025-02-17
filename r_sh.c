#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

// List of processes to terminate in shutdown
const char *processes_to_kill[] = {"sh", "getty", "init"};
const int num_processes = 3;

// Function to execute external commands
void execute_command(char *command) {
    if (strcmp(command, "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    } 
    else if (strcmp(command, "shutdown") == 0) {
        printf("System shutting down...\n");

        // Kill all relevant processes
        for (int i = 0; i < num_processes; i++) {
            pid_t pid = fork();
            if (pid == 0) {  // Child process
                execlp("pkill", "pkill", "-9", processes_to_kill[i], NULL);
                perror("Error executing pkill");
                exit(EXIT_FAILURE);
            } else if (pid > 0) {  // Parent process
                waitpid(pid, NULL, 0);
            } else {
                perror("Error in fork");
            }
        }

        printf("All processes terminated. System shutdown complete.\n");
        exit(0);
    } 
    else {
        // Create a child process to execute the command
        pid_t pid = fork();
        if (pid == 0) {  
            execlp(command, command, NULL);
            perror("Error executing command");
            exit(EXIT_FAILURE);
        } 
        else if (pid > 0) {  
            waitpid(pid, NULL, 0);
        } 
        else {  
            perror("Error in fork");
        }
    }
}

int main() {
    char command[100];

    while (1) {
        printf("sh> ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;  // Handle EOF (Ctrl+D)
        }

        // Remove trailing newline
        command[strcspn(command, "\n")] = 0;

        // Execute the command
        execute_command(command);
    }

    return 0;
}
