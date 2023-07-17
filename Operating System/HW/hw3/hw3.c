/* fork-example-impatient.c */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>

extern long next_thread_number;
extern int max_squares;
extern int total_open_tours;
extern int total_closed_tours;

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
struct Arguments {
    int m, n, r,  c, covered,start_x ,start_y;
    int ** visited;
    int * move_X;
    int * move_Y;
    struct point * pts;
    pthread_t * threads;
    int thread_counter;
};
pthread_mutex_t open_tours = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t closed_tours = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t max_squares_covered = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t next_thread_number_lock = PTHREAD_MUTEX_INITIALIZER;
int isVisited(int ** visited, int r, int c);

int findValidMoves(int m, int n, int r, int c, int *visited,
                   int *move_X, int *move_Y, struct point * pts);

void setVisited(int ** visited, int r, int c);
void * search(void * arg);
int isValid(int x, int y, int board_width, int board_height, int ** visited)
{
    int flag = 1;
    if (x>=board_width || x<0 || y>=board_height || y<0|| isVisited(visited,y,x)==1)
    {
        flag = 0;
    }
    return flag;
}

int ** copyVisited(int m, int n, int ** visited)
{
    int ** new_visited = calloc(m, sizeof(int *));
    for (int i = 0; i < m; i++) {
        *(new_visited + i) = calloc(n, sizeof(int));
    }
    for (int i = 0; i<m;i++)
    {
        for (int j = 0;j<n;j++)
        {
            *(*(new_visited+i)+j) = *(*(visited+i)+j);
        }
    }
    return new_visited;
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
void * search(void * arg) {

    struct Arguments temp = *(struct Arguments *) arg;
    int m = temp.m, n = temp.n, r = temp.r, c = temp.c, covered = temp.covered, start_x = temp.start_x, start_y = temp.start_y;
    int **visited = temp.visited;
    int *move_X = temp.move_X;
    int *move_Y = temp.move_Y;
    struct point *pts = temp.pts;
    pthread_t *threads = temp.threads;
    int thread_counter = temp.thread_counter;
    int valid_pts = 0;
    int next_x;
    int next_y;

    //Try all next moves from the current coordinate dx, dy
    for (int i = 0; i < 8; i++) {
        next_x = c + *(move_X + i);
        next_y = r + *(move_Y + i);
        if (isValid(next_x, next_y, n, m, visited)) {
            struct point temp1 = {next_x, next_y};
            *(pts + valid_pts) = temp1;
            valid_pts++;
        }
    }

    if (covered == m * n) {
        int flag = 1;
        for (int i = 0; i < 8; i++) {
            next_x = c + *(move_X + i);
            next_y = r + *(move_Y + i);
            if (isValidNoVisit(next_x, next_y, n, m)) {
                if (next_x == start_x && next_y == start_y) {
                    if (queit == 1) {
                        if (thread_counter != 0){
                            printf("T%d: Sonny found a closed knight's tour; incremented total_closed_tours\n", thread_counter);
                        }
                        else{
                            printf("MAIN: Sonny found a closed knight's tour; incremented total_closed_tours\n");
                        }
                    }
                    pthread_mutex_lock(&closed_tours);
                    {
                        total_closed_tours++;
                    }
                    pthread_mutex_unlock(&closed_tours);
                    flag = 0;
                }
            }
        }
        if (flag == 1) {
            if (queit == 1) {
                if (thread_counter != 0){
                    printf("T%d: Sonny found an open knight's tour; incremented total_open_tours\n", thread_counter);
                }
                else{
                    printf("MAIN: Sonny found an open knight's tour; incremented total_open_tours\n");
                }
            }
            pthread_mutex_lock(&open_tours);
            {
                total_open_tours++;
            }
            pthread_mutex_unlock(&open_tours);
        }
       return NULL;
    }
    if (valid_pts == 0) {
            if (covered>max_squares)
            {
                if (queit == 1){
                    if (thread_counter != 0) {
                        printf("T%d: Dead end at move #%d; updated max_squares\n", thread_counter, covered);
                    }
                    else {
                        printf("MAIN: Dead end at move #%d; updated max_squares\n", covered);
                    }
                }
                pthread_mutex_lock(&max_squares_covered);
                {
                    max_squares = covered;
                }
                pthread_mutex_unlock(&max_squares_covered);
            }
            else{
                if (queit == 1){
                    if (thread_counter != 0) {
                        printf("T%d: Dead end at move #%d\n", thread_counter, covered);
                    }
                    else {
                        printf("MAIN: Dead end at move #%d\n", covered);
                    }
                }
            }
        }
    else if (valid_pts == 1) {
        setVisited(visited, (*pts).y, (*pts).x);
        struct Arguments arg2 = {m, n, (*pts).y, (*pts).x, covered+1, start_x, start_y, visited, move_X, move_Y,
                                pts, threads, thread_counter};
        search((void *) &arg2);
    } else {
        if (queit == 1) {
            if (thread_counter != 0) {
                printf("T%d: %d possible moves after move #%d; creating %d child threads...\n", thread_counter, valid_pts, covered, valid_pts);
            }
            else {
                printf("MAIN: %d possible moves after move #%d; creating %d child threads...\n", valid_pts, covered, valid_pts);
            }
        }
        for (int i = 0; i < valid_pts; i++) {
            int current_tid = next_thread_number;
            pthread_mutex_lock(&next_thread_number_lock);
            {
                next_thread_number++;
            }
            pthread_mutex_unlock(&next_thread_number_lock);
            int ** new_visited = copyVisited(m,n,visited);
            struct point * new_pts = calloc(8,sizeof(struct point));
            setVisited(new_visited, (*(pts+i)).y, (*(pts+i)).x);
            struct Arguments argument = {m, n, (*(pts+i)).y, (*(pts+i)).x, covered + 1, start_x, start_y, new_visited, move_X,
                                    move_Y, new_pts , threads,current_tid};
            pthread_create((threads+current_tid), NULL, search, (void *) &argument);
//            if (parallel == 0||parallel == 1) {
//                pthread_join(*(threads+current_tid), NULL);
//                if (queit == 1){
//                    if (thread_counter != 0) {
//                        printf("T%d: T%d joined\n", thread_counter, current_tid);
//                    } else {
//                        printf("MAIN: T%d joined\n", current_tid);
//                    }
//                }
//
//                }
            free(new_pts);
            free(new_visited);
            }
        }
    return NULL;
    }
int simulate(int argc, char **argv) {

        setvbuf(stdout, NULL, _IONBF, 0);
        if (argc != 5) {
            fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: hw3.out <m> <n> <r> <c>\n");
            return EXIT_FAILURE;
        }

        for (int i = 1; i < 3; i++) {
            char *temp = *(argv + i);
            if (atoi(temp) < 3) {
                fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: hw3.out <m> <n> <r> <c>\n");
                return EXIT_FAILURE;
            }
        }

        for (int i = 3; i < 5; i++) {
            char *temp = *(argv + i);
            if (atoi(temp) < 0) {
                fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: hw3.out <m> <n> <r> <c>\n");
                return EXIT_FAILURE;
            }
            if (atoi(temp) == 0) {
                if (!isdigit(*temp)) {
                    fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: hw3.out <m> <n> <r> <c>\n");
                    return EXIT_FAILURE;
                }
            }
        }
        int m = atoi(*(argv + 1));
        int n = atoi(*(argv + 2));
        int r = atoi(*(argv + 3));
        int c = atoi(*(argv + 4));
        if (isValidNoVisit(c, r, n, m) == 0) {
            fprintf(stderr, "ERROR: Invalid argument(s)\nUSAGE: hw3.out <m> <n> <r> <c>\n");
            return EXIT_FAILURE;
        }
        int **visited = calloc(m, sizeof(int *));
        for (int i = 0; i < m; i++) {
            *(visited + i) = calloc(n, sizeof(int));
        }
        int *move_Y = calloc(8, sizeof(int));
        *(move_Y + 0) = -2;
        *(move_Y + 1) = -1;
        *(move_Y + 2) = 1;
        *(move_Y + 3) = 2;
        *(move_Y + 4) = 2;
        *(move_Y + 5) = 1;
        *(move_Y + 6) = -1;
        *(move_Y + 7) = -2;
        int *move_X = calloc(8, sizeof(int));
        *(move_X + 0) = 1;
        *(move_X + 1) = 2;
        *(move_X + 2) = 2;
        *(move_X + 3) = 1;
        *(move_X + 4) = -1;
        *(move_X + 5) = -2;
        *(move_X + 6) = -2;
        *(move_X + 7) = -1;
        setVisited(visited, r, c);
        printf("MAIN: Solving Sonny's knight's tour problem for a %dx%d board\n", m, n);
        printf("MAIN: Sonny starts at row %d and column %d (move #1)\n", r, c);
        struct point *pts = calloc(8, sizeof(struct point));
        pthread_t *threads = calloc(1000000, sizeof(pthread_t));
        struct Arguments arg = {m, n, r, c, 1, c, r, visited, move_X, move_Y,
                                pts, threads,0};
        search((void *) &arg);
        if (total_open_tours > 0 || total_closed_tours > 0) {
            printf("MAIN: Search complete; found %d open tours and %d closed tours\n", total_open_tours, total_closed_tours);
        } else {
            printf("MAIN: Search complete; best solution(s) visited %d squares out of %d\n", max_squares, m * n);
        }
        for (int i = 0; i < m; i++) {
            free(*(visited + i));
        }
        free(visited);
        free(move_Y);
        free(move_X);
        free(threads);
        free(pts);
        return EXIT_SUCCESS;
    }
void setVisited(int ** visited, int r, int c)
{*(*(visited+r)+c) = 1;}

