#include <stdlib.h>         
#include <unistd.h>         /* Needed for execlp, etc. */
#include <stdio.h>          /* Needed for etc. */

int main(int argc, char** argv) {
    int i;

    /*
     * Iterate from 1 to the Nth argument.
     * Create a 
     */
    for (i=1; i<argc-1; ++i) {
        int pd[2];
        pipe(pd);

        if (!fork()) {
            dup2(pd[1], 1);
            execlp(argv[1], argv[i], NULL);
            perror("exec");
            abort();
        }

        dup2(pd[0], 0);
        close(pd[1]);
    }

    execlp(argv[i], argv[i], NULL);
    perror("exec");
    abort();
}
