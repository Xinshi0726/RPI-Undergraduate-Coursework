/* pipe-ipc.c */

/* A "pipe" is a unidirectional communication channel -- man 2 pipe */
/*               ^^^                                                */

/* TO DO: Draw a diagram that shows all possible outputs for this code */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int pipe_test(int pipefd[2],int children){
    int * a = calloc(100,sizeof(int));
    for(int i = 0;i<children;i++){
        pid_t p = fork();  /* fork() will copy the fd table to the child */
        if ( p == -1 )
        {
            perror( "fork() failed" );
            return EXIT_FAILURE;
        }

        if ( p == 0 )   /* CHILD process */
        {
            close( pipefd[0] );   /* close the "read" end of the pipe */
            /* write data to the pipe */
            int bytes_written = write( pipefd[1], "1", 1 );
/*----CONTEXT SWITCH from CHILD process to PARENT process------------------*/
            printf( "CHILD: Wrote %d bytes to the pipe\n", bytes_written );
            pipe_test(pipefd,children-1);
            exit(0);
        }

    }
    int status;  /* exit status from each child process */
    while ( children > 0 )
    {
        pid_t child_pid = waitpid( -1, &status, 0 );   /* BLOCKING CALL */
        /*                         ^^
                               wait for any of my child processes */

        children--;

        printf( "PARENT: child process %d terminated...", child_pid );

        if ( WIFSIGNALED( status ) )  /* child process was terminated   */
        {                             /*  by a signal (e.g., seg fault) */
            printf( "abnormally (killed by a signal)\n" );
        }
        else if ( WIFEXITED( status ) )
        {
            int exit_status = WEXITSTATUS( status );
            printf( "normally with exit status %d\n", exit_status );
        }
    }

}
int main()
{


    /* fd table:

       0: stdin
       1: stdout
       2: stderr                   +--------+
       3: pipefd[0] <====READ======| buffer | think of this buffer as a
       4: pipefd[1] =====WRITE====>| buffer |  temporary hidden file...
                                   +--------+
     */
    int pipefd[2];  /* array to hold the two pipe (file) desciptors:
                     pipefd[0] is the "read" end of the pipe
                     pipefd[1] is the "write" end of the pipe */

    /* create a pipe */
    int rc = pipe( pipefd );  /* the input argument pipefd will be
                               populated with the OS-assigned
                               read and write pipe/file descriptors */

    if ( rc == -1 )
    {
        perror( "pipe() failed" );
        return EXIT_FAILURE;
    }
    pipe_test(pipefd,3);
    char buffer[20];
    int bytes_read = read( pipefd[0], buffer, 20 );
    buffer[bytes_read] = '\0';
    printf( "Read %d bytes: \"%s\"\n", bytes_read, buffer );

    close( pipefd[0] );
      /* close the "read" end of the pipe */
    return EXIT_SUCCESS;
}
