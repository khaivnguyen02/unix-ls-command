#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1024

int main() {
    char command[MAX_COMMAND_LENGTH];
    char dir[1024];
    int list_long, show_inode;

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
        list_long = 0;
        show_inode = 0;

        char *options = strstr(command, "-");
        if (options) {
            for (int i = 1; options[i] != ' ' && options[i] != '\0'; i++) {
                if (options[i] == 'l') list_long = 1;
                if (options[i] == 'i') show_inode = 1;
            }
        }

        if (!options) { // No options provided
            strcpy(dir, "."); // Default to current directory
        }

        simulate_ls(dir, list_long, show_inode);
    }

    printf("Exiting SimpleShell.\n");
    return 0;
}
