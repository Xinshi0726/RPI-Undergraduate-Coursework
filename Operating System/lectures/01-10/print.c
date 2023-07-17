#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/file.h>

int main() {
//        close( 2 );
//        int fd = open( "q7.txt", O_WRONLY | O_CREAT | O_TRUNC, 0640 );
        printf( ">Q7\n" );
        pid_t p = fork();
//        if ( p == 0 ) printf( "<YY%d>", getpid() );
//        if ( p > 0 )
//        {
//            fprintf( stderr, "{XX%d}", p );
//            int * status = malloc( sizeof( int ) );
//            pid_t child_pid = 0;
//            while ( child_pid == 0 )
//            {
//                child_pid = waitpid( p, status, 0 );
//                if ( child_pid == 0 ) { printf( "-ZZ" ); sleep( 1 ); }
//            }
//            printf( "(%d)", WEXITSTATUS( *status ) );
//        }
//        printf( "..%d\n", getpid() );
//        close( fd );
        return EXIT_SUCCESS;
}
