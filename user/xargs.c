#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
    char *new_argv[MAXARG];
    for(int i = 0; i < argc; i++)
    {
        new_argv[i] = malloc(strlen(argv[i]) + 1); 
        strcpy(new_argv[i], argv[i]);
        // printf("copy argv[%d] %s to %s\n", i, argv[i], new_argv[i]);
    }
    int p = argc;
    char buf;
    char temp[512];
    int temp_index = 0;
    for(;;)
    {
        int n = read(0, &buf, 1); // Read one character at a time
        if(n == 0) break; // Break if no more characters to read

        if(buf == '\n') { // Check if the character is a newline
            temp[temp_index] = '\0'; // Terminate the string
            new_argv[p] = malloc(strlen(temp) + 1); // Allocate memory
            strcpy(new_argv[p++], temp); // Copy the temp string into new_argv
            // printf("get buf: %s\n", temp); // Print the string
            temp_index = 0; // Reset the temp_index for the next line
        } else {
            temp[temp_index++] = buf; // Store the character in temp
        }
    }
    if(fork() == 0)
    {
        exec(argv[1], new_argv + 1);
    }
    else
    {
        wait(0);
    }
    exit(0);
}
