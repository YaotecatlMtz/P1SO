#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define NUM_GETTY 6

void spawn_getty() {
    if (fork() == 0) {
        execlp("xterm", "xterm", "-e", "./getty", NULL);
        perror("execlp");
        exit(1);
    }
}

void handle_shutdown(int sig) {
    // Manejar la señal de shutdown
    printf("Init recibió la señal de shutdown. Terminando...\n");
    exit(0);
}

int main() {
    pid_t getty_pids[NUM_GETTY];
    int i;

    // Configurar el manejador de señales para SIGTERM
    signal(SIGTERM, handle_shutdown);

    // Crear 6 procesos getty
    for (i = 0; i < NUM_GETTY; i++) {
        spawn_getty();
    }

    // Verificar que los procesos getty estén en ejecución
    while (1) {
        int status;
        pid_t pid = waitpid(-1, &status, WNOHANG);

        if (pid > 0) {
            // Un proceso getty terminó, crear uno nuevo
            spawn_getty();
        }

        sleep(1); // Esperar un segundo antes de verificar nuevamente
    }

    return 0;
}