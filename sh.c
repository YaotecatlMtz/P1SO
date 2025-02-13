#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void execute_command(char *command, int background) {
    pid_t pid = fork();

    if (pid == 0) {
        // Proceso hijo
        execlp(command, command, NULL);
        perror("execlp");
        exit(1);
    } else if (pid > 0 && !background) {
        // Proceso padre (si no es en segundo plano, esperar al hijo)
        waitpid(pid, NULL, 0);
    }
}

int main() {
    char input[256];

    while (1) {
        printf("sh > ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Eliminar el salto de línea

        if (strcmp(input, "exit") == 0) {
            break; // Salir del shell
        } else if (strcmp(input, "shutdown") == 0) {
            // Terminar todos los procesos
            kill(0, SIGTERM);
            break;
        } else {
            // Ejecutar comando
            int background = 0;
            if (input[strlen(input) - 1] == '&') {
                background = 1;
                input[strlen(input) - 1] = '\0'; // Eliminar el '&'
            }
            execute_command(input, background);
        }
    }

    return 0;
}