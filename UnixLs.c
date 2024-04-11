#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1024

int main() {
    char command[MAX_COMMAND_LENGTH];
    char cmd[128];

    while (1) {
        printf("$ ");  // Prompt for input
        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("fgets failed");
            exit(1);
        }

        // Remove newline character if present
        command[strcspn(command, "\n")] = 0;

        // Check for the exit command
        if (strcmp(command, "exit") == 0) {
            break;
        }

        // Validate and build the ls command
        int l = 0;
        int i = 0;
        for (int j = 2; j < 6; ++j) {
            if (command[j] == 'l') l = 1;
            else if (command[j] == 'i') i = 1;
        }
    }

    printf("Exiting SimpleShell.\n");
    return 0;
}
