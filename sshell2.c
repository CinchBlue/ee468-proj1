/*
 *  This is a simple shell program from
 *  rik0.altervista.org/snippetss/csimpleshell.html
 *  It's been modified a bit and comments were added.
 *
 *  It doesn't allow misdirection, e.g., <, >, >>, or |
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFFER_SIZE 80
#define ARR_SIZE 80

// #define DEBUG 1  /* In case you want debug messages */


int parse_delim_pipe(char *buffer, char** args, 
                size_t args_size, size_t *nargs)
{
/* 
 * size_t data type is defined in the 1999 ISO C standard (C99).
 * It is used to represent the sizes of objects. size_t is the
 * preferred way to declare arguments or variables that hold the
 * size of an object.
 */
    char *buf_args[args_size]; /* You need C99.  Note that args_size
                                  is normally a constant. */
    char **cp;  /* This is used as a pointer into the string array */
    char *wbuf;  /* String variable that has the command line */
    size_t i, j; 
    
    wbuf=buffer;
    buf_args[0]=buffer; 
    args[0] =buffer;
/*
 * Now 'wbuf' is parsed into the string array 'buf_args'
 *
 * The for-loop uses a string.h function
 *   char *strsep(char **stringp, const char *delim);
 *
 *   Description:  
 *   If *stringp = NULL then it returns NULL and does
 *   nothing else.  Otherwise the function finds the first token in
 *   the string *stringp, where tokens are delimited by symbols
 *   in the string 'delim'.  
 *
 *   In the example below, **stringp is &wbu, and 
 *   the delim = ' ', '\n', and '\t'.  So there are three possible 
 *   delimiters. 
 *
 *   So in the string " Aloha World\n", the spaces and "\n" are
 *   delimiters.  Thus, there are three delimiters.  The tokens
 *   are what's between the delimiters.  So the first token is
 *   "", which is nothing because a space is the first delimiter.
 *   The second token is "Aloha", and the third token is "World".
 *   
 *   The function will scan a character string starting from
 *   *stringp, search for the first delimiter.  It replaces
 *   the delimiter with '\0', and *stringp is updated to point
 *   past the token.  In case no delimiter was found, the
 *   token is taken to be the entire string *stringp, and *stringp
 *   is made NULL.   Strsep returns a pointer to the token. 
 *
 *   Example:  Suppose *stringp -> " Aloha World\n"
 *
 *   The first time strsep is called, the string is "\0Aloha World\n",
 *   and the pointer value returned = 0.  Note the token is nothing.
 *
 *   The second time it is called, the string is "\0Aloha\0World\n",
 *   and the pointer value returned = 1  Note that 'Aloha' is a token.
 *
 *   The third time it is called, the string is '\0Aloha\0World\0', 
 *   and the pointer value returned is 7.  Note that 'World' is a token.
 *
 *   The fourth time it is called, it returns NULL.
 *
 *   The for-loop, goes through buffer starting at the beginning.
 *   wbuf is updated to point to the next token, and cp is
 *   updated to point to the current token, which terminated by '\0'.
 *   Note that pointers to tokens are stored in array buf_args through cp.
 *   The loop stops if there are no more tokens or exceeded the
 *   array buf_args.
 */   
    /* cp is a pointer to buff_args */ 
    int num_proc = 0;
    /* Creates pointers to each of the arguments */
    for(cp=buf_args; (*cp=(char*)strsep(&wbuf, "|")) != NULL ;){
        ++num_proc;
        if ((*cp != '\0') && (++cp >= &buf_args[args_size]))
            break; 
    }

/* 
 * Copy 'buf_args' into 'args'
 */    
    for (j=i=0; buf_args[i]!=NULL; i++){ 
        if(strlen(buf_args[i])>0)  /* Store only non-empty tokens */
            args[j++]=buf_args[i];
    }
    
    *nargs=j;
    args[j]=NULL;
    return num_proc;
}


void parse_args(char *buffer, char** args, 
                size_t args_size, size_t *nargs)
{
    char *buf_args[args_size]; /* You need C99.  Note that args_size
                                  is normally a constant. */
    char **cp;  /* This is used as a pointer into the string array */
    char *wbuf;  /* String variable that has the command line */
    size_t i, j; 
    
    wbuf=buffer;
    buf_args[0]=buffer; 
    args[0] =buffer;
    /* cp is a pointer to buff_args */ 

    /* Creates pointers to each of the arguments */
    for(cp=buf_args; (*cp=strsep(&wbuf, " \n\t")) != NULL ;){
        if ((*cp != '\0') && (++cp >= &buf_args[args_size]))
            break; 
    }

    for (j=i=0; buf_args[i]!=NULL; i++){ 
        if(strlen(buf_args[i])>0)  /* Store only non-empty tokens */
            args[j++]=buf_args[i];
    }
    
    *nargs=j;
    args[j]=NULL;
}


/*
 *
 * IDEA:
 * - Create a function just to handle the execution.
 * - Have the parent/child establish the pipes.
 *
 * - Redirect stdin and stdio to the pipes.
 *
 */

struct exec_info {
    char* str_args[ARR_SIZE];
    size_t num_args;
};

/*
 *
 *
 *
 */


int main(int argc, char *argv[], char *envp[]){
    char buffer[BUFFER_SIZE];
    char* args[ARR_SIZE];

    int* ret_status;
    size_t nargs;
    
    while(1){
        printf("ee468>> "); /* Prompt */
        fgets(buffer, BUFFER_SIZE, stdin); /* Read in command line */
              /* Parse the command line into args */
        int num_exec = parse_delim_pipe(buffer, args, ARR_SIZE, &nargs); 

        struct exec_info execinfo[num_exec];

        /*
         * For each |-deliminated string, split further.
         * Then, assign the information into its struct exec_info
         */
        size_t i;
        for(i = 0; i < num_exec; ++i) {
            /* get the string */
            char* it_str = args[i]; 
            /* let's map our outputs onto exec_info struct object... */
                /* 
                 * void parse_args(char* buffer,        INPUT
                 *                 char** args,         OUTPUT arr<char* const> 
                 *                 size_t args_size,    INPUT
                 *                 size_t *nargs)       OUTPUT
                 */
            /*
             * We also need to make sure that we have execinfo[i].str_args buffer
             * ready. It's in the definition for struct exec_info now--it has a size of
             * ARGS_SIZE or so.
             */
            parse_args(it_str, execinfo[i].str_args, ARR_SIZE, &execinfo[i].num_args);
#ifdef DEBUG
            printf("exec%2zu: num args: %zu\n", i,  execinfo[i].num_args);
#endif
        }

#ifdef DEBUG
        /*
         * DEBUG: Print out the exec() number, then the arguments involved.
         */
        size_t it;
        for (it = 0; it < num_exec; ++it) {
            printf("(%zu):\n", it);
            
            /*
             * Print out each argument.
             */
            size_t jt;
            for (jt = 0; jt < execinfo[it].num_args; ++jt) {
                printf("\t(%zu): %s\n", jt, execinfo[it].str_args[jt]);
            }
        }
#endif
 
        if (nargs==0) continue; /* Nothing entered so prompt again */
        if (!strcmp(args[0], "exit" )) exit(0);       

        /*
         *
         * BEGIN EXECUTION OF PROCESSES
         *
         */
        /* okay! */
        /* ALGORITHM:
         * 1. Setup in/out pipes.
         * 2. Duplicate pipes onto stdin and out as needed.
         * 3. Setup process A (where A is our current process)
         * 4. Execute process A
         * 5. Wait until A is done, then clean it up.
         * 6. Do such that the next process B (where
         *    B is the next process in line) is setup where
         *    it will be A on the next iteration
         * 7. Iterate
         */

        /* So the way I have it coded is to keep one "global"
         * pipe_in pipe. This is because the out-going pipe will
         * need to become the in-going pipe when we take the
         * output of process A into the pipe which needs to go into B,
         * where B will replace the "position" of A on the next
         * iteration.
         */
        int pipe_in[2];
        pipe(pipe_in);
        /* For the first iteration, the first program will
         * take input from stdin.
         */
        dup2(0, pipe_in[1]);
        /* proc_it is the loop iterator variable. */
        size_t proc_it;
        for (proc_it = 0; proc_it < num_exec; ++proc_it) {
            pid_t pid;
            /* Intialize the out-going pipe. */ 
            int pipe_out[2];
            pipe(pipe_out);

            /* If it's the first command, take input from stdin
             * as needed. Otherwise, close the input side of the in-going pipe.
             */
            if (proc_it != 0) {
                /* Close pipe_in.in */
                close(pipe_in[1]);
            }

            /* Fork into parent, child. */
            pid = fork();
            if (pid == 0){  /* The child */
                /* Close the out side of the 
                /* out-going pipe.
                 */
                close(pipe_out[0]);

                /* Duplicate pipe_in.out onto A.stdin
                 *           A.stdout onto pipe_out.in
                 *           A.stderr onto pipe_out.in
                 */

                /* Run the program using the correct
                 * <proc_it>-th exec() info.
                 */
                if (execvp(execinfo[proc_it].str_args[0],
                           execinfo[proc_it].str_args)) {
                    puts(strerror(errno));
                    exit(127);
                }
            } else { /* The parent */

                #ifdef DEBUG
                    printf("Waiting for child (%d)\n", pid);
                #endif



                pid = wait(ret_status);


                #ifdef DEBUG
                    printf("Child (%d) finished\n", pid);
                #endif
            } 

        }
    }    
    return 0;
}


