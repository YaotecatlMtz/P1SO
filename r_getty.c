// getty.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PASSWD_FILE "passwd"

void start_shell() {
    execl("./sh", "sh", NULL);
    perror("execl");
    exit(EXIT_FAILURE);
}

int authenticate(char *username, char *password) {
    FILE *file = fopen(PASSWD_FILE, "r");
    if (!file) {
        perror("fopen");
        return 0;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *stored_user = strtok(line, ":");
        char *stored_pass = strtok(NULL, "\n");
        if (stored_user && stored_pass && strcmp(username, stored_user) == 0 && strcmp(password, stored_pass) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int main() {
    char username[50], password[50];
    while (1) {
        printf("login: ");
        scanf("%s", username);
        printf("password: ");
        scanf("%s", password);
        if (authenticate(username, password)) {
            printf("Login successful!\n");
            start_shell();
        } else {
            printf("Invalid login. Try again.\n");
        }
    }
    return 0;
}