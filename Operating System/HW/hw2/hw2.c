/* fork-example-impatient.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>

#ifndef QUIET
static const int queit = 1;
#else
static const int queit = 0;
#endif

#ifndef NO_PARALLEL
static const int parallel = 1;
#else
static const int parallel = 0;
#endif

struct point{
    int x;
    int y;
};
int isVisited(int ** visited, int r, int c);

int findValidMoves(int m, int n, int r, int c, int *visited,
                   int *move_X, int *move_Y, struct point * pts);

void setVisited(int ** visited, int r, int c);

int search(int m, int n, int r, int c,int covered, int ** visited, int * move_X,int * move_Y, int * pipefd, int start_x ,int start_y,struct point * pts,pid_t * pids);

int isValid(int x, int y, int board_width, int board_height, int ** visited)
{
    int flag = 1;
    if (x>=board_width || x<0 || y>=board_height || y<0|| isVisited(visited,y,x)==1)
    {
        flag = 0;
    }
    return flag;
}

int isValidNoVisit(int x, int y, int board_width, int board_height)
{
    int flag = 1;
    if (x>=board_width || x<0 || y>=board_height || y<0)
    {
        flag = 0;
    }
    return flag;
}
int isVisited(int ** visited, int r, int c){
    if (*(*(visited+r)+c) == 1){
        return 1;
    }
    return 0;
}
int search(int m, int n, int r, int c,int covered, int ** visited, int * move_X,int * move_Y, int * pipefd, int start_x ,int start_y,struct point * pts, pid_t * pids){
    //if the squares covered equals to the total squares, write to the pipe to notify the top-level parent process
    //Find all valid moves given the current position
    //If there's no move, return the number of squares covered
    //If there is one move, just do a regular search
    //If there are multiple moves, create fork
    //CHILD: solve the corresponding sub-problem, return cells covered.
    //PARENT:

    int valid_pts = 0;

    int next_x;
    int next_y;

    //Try all next moves from the current coordinate dx, dy
    for(int i=0; i < 8; i++)
    {
        next_x = c + *(move_X+i);
        next_y = r + *(move_Y+i);
        if(isValid(next_x,next_y,n,m,visited)){
            struct point temp = {next_x, next_y};
            *(pts + valid_pts) = temp;
            valid_pts++;
        }
    }

    if (covered == m*n)
    {
        int flag = 1;
        for(int i=0; i < 8; i++)
        {
            next_x = c + *(move_X+i);
            next_y = r + *(move_Y+i);
            if(isValidNoVisit(next_x,next_y,n,m)){
                if (next_x == start_x && next_y == start_y)
                {
                    if (queit == 1){
                        printf("PID %d: Sonny found a closed knight's tour; notifying top-level parent\n", getpid());}
                    write( *(pipefd+1), "0", 1 );
                    flag = 0;
                }
            }
        }
        if (flag == 1){
            if (queit == 1){
                printf("PID %d: Sonny found an open knight's tour; notifying top-level parent\n", getpid());}
            write( *(pipefd+1), "1", 1 );
        }

        close( *(pipefd+0) );
        close( *(pipefd+1) );
        free(pipefd);
        pipefd = NULL;
        free(pts);
        pts=NULL;
        free(pids);
        pids =NULL;
        for(int i =0; i < m; i++){
            free(*(visited+i));
            *(visited+i) = NULL;
        }
        free(visited);
        visited = NULL;
        free(move_Y);
        move_Y = NULL;
        free(move_X);
        move_X = NULL;
        exit(covered);
    }
    if (valid_pts == 0){
        if (queit == 1){
            printf("PID %d: Dead end at move #%d\n", getpid(), covered);}
        close( *(pipefd+0) );
        close( *(pipefd+1) );
        free(pipefd);
        pipefd = NULL;
        free(pts);
        pts=NULL;
        free(pids);
        pids =NULL;
        for(int i =0; i < m; i++){
            free(*(visited+i));
            *(visited+i) = NULL;
        }
        free(visited);
        visited = NULL;
        free(move_Y);
        move_Y = NULL;
        free(move_X);
        move_X = NULL;
        exit(covered);
    }
    else if (valid_pts == 1){
        setVisited(visited,(*(pts)).y,(*(pts)).x);
        int temp = search(m,n,(*(pts)).y,(*(pts)).x,covered+1, visited, move_X, move_Y,pipefd,start_x,start_y,pts,pids);
        close( *(pipefd+0) );
        close( *(pipefd+1) );
        free(pipefd);
        pipefd = NULL;
        free(pts);
        pts=NULL;
        free(pids);
        pids =NULL;
        for(int i =0; i < m; i++){
            free(*(visited+i));
            *(visited+i) = NULL;
        }
        free(visited);
        visited = NULL;
        free(move_Y);
        move_Y = NULL;
        free(move_X);
        move_X = NULL;
        exit(temp);
    }
    else{
        if (queit == 1){
            printf("PID %d: %d possible moves after move #%d; creating %d child processes...\n", getpid(), valid_pts, covered, valid_pts);}
        int temp1 = 0;
        for (int i = 0; i < valid_pts; i++){
            int tpid = fork();

            if (tpid < 0){
                perror("Fork failed.\n");
            }

            else if (tpid == 0){
                close(*(pipefd+0));
                setVisited(visited,(*(pts+i)).y,(*(pts+i)).x);
                int temp = search(m,n,(*(pts+i)).y,(*(pts+i)).x,covered+1, visited, move_X, move_Y, pipefd,start_x,start_y,pts,pids);

                close( *(pipefd+0) );
                close( *(pipefd+1) );
                free(pipefd);
                pipefd = NULL;
                free(pts);
                pts=NULL;
                free(pids);
                pids =NULL;
                for(int i =0; i < m; i++){
                    free(*(visited+i));
                    *(visited+i) = NULL;
                }
                free(visited);
                visited = NULL;
                free(move_Y);
                move_Y = NULL;
                free(move_X);
                move_X = NULL;
                exit(temp);
            }
            else if (tpid>0 && parallel == 0){
                *(pids + i) = tpid;
                int status;
                waitpid(tpid,&status,0);
                if(WIFSIGNALED(status)){
                    printf( "abnormally\n"); /*core dump or kill */
                    fflush(stdout);
                }
                else if( WIFEXITED(status)){
                    int rc = WEXITSTATUS (status);
                    if (rc>temp1)
                    {
                        temp1 = rc;
                    }
                }
            }
        }
        close( *(pipefd+1) );
        if (parallel == 1)
        {
            close( *(pipefd+1) );
            int children = valid_pts;
            while(children > 0){

                int status; //return status for each child process

                /*Wait until a child process extis */
                waitpid(*(pids+(valid_pts-children)),&status,0); /* BLOCK */

                if(WIFSIGNALED(status)){
                    printf( "abnormally\n"); /*core dump or kill */
                    fflush(stdout);
                }
                else if( WIFEXITED(status)){
                    int rc = WEXITSTATUS (status);
                    if (rc>temp1)
                    {
                        temp1 = rc;
                    }
                }
                children--;
            }
        }
        return(temp1);

    }
    free(pids);
    pids = NULL;
    free(pts);
    pts = NULL;
}
int main( int argc, char ** argv )
{

    setvbuf( stdout, NULL, _IONBF, 0 );
    if ( argc != 5 )
    {
        fprintf( stderr, "ERROR: Invalid argument(s)\nUSAGE: hw2.out <m> <n> <r> <c>\n" );
        return EXIT_FAILURE;
    }

    for (int i = 1; i < 3; i++){
        char* temp = *(argv + i);
        if (atoi(temp) < 3){
            fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: hw2.out <m> <n> <r> <c>\n");
            return EXIT_FAILURE;
        }
    }

    for (int i = 3; i < 5; i++){
        char* temp = *(argv + i);
        if (atoi(temp) < 0){
            fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: hw2.out <m> <n> <r> <c>\n");
            return EXIT_FAILURE;
        }
        if (atoi(temp) == 0){
            if (!isdigit(*temp)){
                fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: hw2.out <m> <n> <r> <c>\n");
                return EXIT_FAILURE;
            }
        }
    }
    int m = atoi(*(argv+1));
    int n = atoi(*(argv+2));
    int r = atoi(*(argv+3));
    int c = atoi(*(argv+4));
    if (isValidNoVisit(c,r,n,m) == 0)
    {
        fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: hw2.out <m> <n> <r> <c>\n");
        return EXIT_FAILURE;
    }
    int ** visited =  calloc(m, sizeof(int*));
    for(int i =0; i < m; i++){
        *(visited+i) = calloc(n,sizeof(int));
    }
    int * move_Y = calloc(8,sizeof(int));
    *(move_Y+0) = -2; *(move_Y+1) = -1; *(move_Y+2) = 1;*(move_Y+3) = 2; *(move_Y+4) = 2; *(move_Y+5) = 1; *(move_Y+6) = -1; *(move_Y+7) = -2;
    int * move_X = calloc(8,sizeof(int));
    *(move_X+0) = 1; *(move_X+1) = 2; *(move_X+2) = 2;*(move_X+3) = 1; *(move_X+4) = -1; *(move_X+5) = -2; *(move_X+6) = -2; *(move_X+7) = -1;
    setVisited(visited,r,c);
    int * pipefd = calloc(2,sizeof(int));
    int rc = pipe( pipefd );  /* the input argument pipefd will be
                               populated with the OS-assigned
                               read and write pipe/file descriptors */
    if ( rc == -1 )
    {
        perror( "pipe() failed" );
        return EXIT_FAILURE;
    }
    printf("PID %d: Solving Sonny's knight's tour problem for a %dx%d board\n", getpid(), m, n);
    printf("PID %d: Sonny starts at row %d and column %d (move #1)\n", getpid(), r, c);
    struct point * pts = calloc(8,sizeof(struct point));
    pid_t * pids = calloc( 8, sizeof( pid_t ) );
    int covered = search(m,n,r,c,1, visited, move_X, move_Y,pipefd,c,r,pts,pids);
    char * buffer = calloc(1000,sizeof(char));
    int bytes_read = read( *(pipefd+0), buffer, 1000 );
    *(buffer+bytes_read) = '\0';
    int count0 = 0, count1 = 0;
    if (bytes_read != -1)
    {
        for (int i = 0; i < bytes_read; i++) {
            if (*(buffer+i) == '1') {
                count1++;
            }
            if (*(buffer+i) == '0') {
                count0++;
            }
        }
    }
    if (count1>0 || count0>0){
        printf("PID %d: Search complete; found %d open tours and %d closed tours\n", getpid(), count1, count0);
    }
    else{
        printf("PID %d: Search complete; best solution(s) visited %d squares out of %d\n", getpid(), covered , m * n);
    }
    for(int i =0; i < m; i++){
        free(*(visited+i));
    }
    close( *(pipefd+0) );
    close( *(pipefd+1) );
    free(visited);
    free(pipefd);
    free(move_Y);
    free(move_X);
    free(pids);
    free(pts);
    free(buffer);
#if 0
    struct point * pts = calloc(m * n, sizeof(struct point));
    int valid_pts = findValidMoves(n, m, c, r, visited, move_X,
                                   move_Y, pts);
    for (int i = 0; i<valid_pts; i++)
    {
        printf("Parent valid_point: %d %d\n",(*(pts+i)).x,(*(pts+i)).y);
    }
#endif

//    search(m, n, visited, move_X, move_Y, 1);
    return EXIT_SUCCESS;
}

void setVisited(int ** visited, int r, int c)
{*(*(visited+r)+c) = 1;}

