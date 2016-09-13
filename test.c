#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

/*
 * Outputs an array of structs ready to be parsed into exec()
 *         as well as the number of arguments into num_cmd
 */

int main(int argc, char** argv) {
    
    pid_t pid;

    pid = fork();

    
    char* str[80];
    str[0] = "ls";
    str[1] = NULL;

    if (pid == 0) {
        execvp(str[0], str);
    } else {
        int return_status;
        wait(&return_status);
        
        if (return_status == -1) {
            puts(strerror(errno));
        }
    }

    return 0;
}
