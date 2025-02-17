#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

// Function to shut down the system properly
void shutdown_system() {
    printf("System shutting down...\n");

    // Kill init first so it stops restarting processes
    pid_t pid = fork();
    if (pid == 0) {
        execlp("pkill", "pkill", "-9", "init", NULL);
        perror("Error terminating init");
        exit(EXIT_FAILURE);
    }
    waitpid(pid, NULL, 0);

    // Kill remaining processes
    const char *processes_to_kill[] = {"sh", "getty"};
    for (int i = 0; i < 2; i++) {
        pid = fork();
        if (pid == 0) {
            execlp("pkill", "pkill", "-9", processes_to_kill[i], NULL);
            perror("Error terminating process");
            exit(EXIT_FAILURE);
        }
        waitpid(pid, NULL, 0);
    }

    printf("Shutdown complete. All processes terminated.\n");
    exit(0);
}

// Function to execute commands
void execute_command(char *command) {
    if (strcmp(command, "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    } 
    else if (strcmp(command, "shutdown") == 0) {
        shutdown_system();
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
            break; // Handle EOF (Ctrl+D)
        }

        // Remove trailing newline
        command[strcspn(command, "\n")] = 0;

        // Execute the command
        execute_command(command);
    }

    return 0;
}

    return 0;
}
