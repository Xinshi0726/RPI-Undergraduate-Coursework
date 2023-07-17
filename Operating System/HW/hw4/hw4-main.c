/* hw4-main.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <signal.h>

#define SEVEN 10
int main() {
    char buffer[SEVEN];
    int sd = socket( PF_INET, SOCK_STREAM, 0 );
    struct sockaddr_in server;
    server.sin_family = PF_INET;
    server.sin_addr.s_addr = htonl( INADDR_ANY );
    server.sin_port = htons( 7777 );
    bind( sd, (struct sockaddr *)&server, sizeof( server ) );
    listen( sd, 7 );
    struct sockaddr_in client;
    int i, n, len = sizeof( client );
    int newsd = accept( sd, (struct sockaddr *)&client, (socklen_t *)&len );
    do
    {
        n = recv( newsd, buffer, SEVEN - 1, 0 );
        fprintf( stderr, "[" );
        for ( i = 0 ; i < n ; i += 2 ) fprintf( stderr, "%c", buffer[i] );
        fprintf( stderr, "]" );
    }
    while ( n > 0 );
    return EXIT_SUCCESS;
}
