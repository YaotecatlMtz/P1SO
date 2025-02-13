#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define PASSWD_FILE "passwd"
#define MAX_LENGTH 256

int validate_credentials(const char *username, const char *password) {
    FILE *file = fopen(PASSWD_FILE, "r");
    if (!file) {
        perror("fopen");
        return 0;
    }

    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char stored_username[MAX_LENGTH], stored_password[MAX_LENGTH];
        sscanf(line, "%[^:]:%s", stored_username, stored_password);

        if (strcmp(username, stored_username) == 0 && strcmp(password, stored_password) == 0) {
            fclose(file);
            return 1; 
        }
    }

    fclose(file);
    return 0; 
}

void spawn_shell() {
    if (fork() == 0) {
        execlp("./sh", "sh", NULL);
        perror("execlp");
        exit(1);
    } else {
        wait(NULL); // Esperar a que el shell termine
    }
}

int main() {
    char username[MAX_LENGTH], password[MAX_LENGTH];

    while (1) {
        printf("login: ");
        scanf("%s", username);
        printf("password: ");
        scanf("%s", password);

        if (validate_credentials(username, password)) {
            printf("Login successful!\n");
            spawn_shell();
        } else {
            printf("Invalid credentials. Try again.\n");
        }
    }

    return 0;
}