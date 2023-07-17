//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <string.h>
//#include <ctype.h>
//#include <pthread.h>
//#include <signal.h>
//#define MAXBUFFER 16
//
//extern int total_guesses;
//extern int total_wins;
//extern int total_losses;
//extern char ** words;
//int record_word_count = 0;
//void * wordle_one_thread(void * arg);
//
//void * block_on_accept(void * arg);
//
//#define MAX_WORDS 10000
//#define MAX_WORD_LENGTH 100
//pthread_mutex_t total_guesses_lock = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t total_wins_lock = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t total_losses_lock = PTHREAD_MUTEX_INITIALIZER;
//pthread_t *threads;
//pthread_t tid;
//long next_thread_id = 0;
//int break_out = 0;
//typedef struct
//{
//    int newsd;
//    char * right_word;
//    pthread_t tid;
//    char ** dict_words;
//    int num_words;
//} wordle_msg_t;
//typedef struct
//{
//    int num_words; char **dict_words; int listener;
//} block_on_accept_msg_t;
//void signal_handler( int sig )
//{
//    if ( sig == SIGUSR1 )
//    {
//        printf(" SIGUSR1 rcvd; Wordle server shutting down...\n");
//        for ( int i = 0 ; i < next_thread_id ; i++ )
//        {
//            pthread_detach( *(threads+i) );
//        }
//        printf(" Wordle server shutting down...\n");
//        break_out = 1;
//        pthread_detach(tid);
//    }
//}
//
//int wordle_server( int argc, char ** argv )
//{
//    int listener_port = atoi(*(argv+1));
//    int seed = atoi(*(argv+2));
//    char * word_filename = *(argv+3);
//    int num_words = atoi(*(argv+4));
//    signal( SIGINT, SIG_IGN );signal( SIGUSR2, SIG_IGN );signal( SIGUSR1, signal_handler );signal(SIGTERM, SIG_IGN);
//    setvbuf( stdout, NULL, _IONBF, 0 );
//    FILE *file;
//    // Open the file in read mode
//    file = fopen(word_filename, "r");
//    // Check if the file was opened successfully
//    if (file == NULL) {
//        printf("Error: Unable to open the file.\n");
//        return 1;
//    }
//    else{
//        printf("MAIN: opened %s (%d words)\n", word_filename, num_words);
//    }
//    // Dynamically allocate memory for an array of strings
//    char ** dict_words = calloc(MAX_WORDS, sizeof(char *));
//    for (int i = 0; i < MAX_WORDS; i++) {
//        *(dict_words + i) = calloc(MAX_WORD_LENGTH, sizeof(char));
//    }
//
//    // Buffer to store each line from the file
//    char * buffer = calloc(MAX_WORD_LENGTH, sizeof(char));
//    // Read each line and store it in the words array
//    int word_count = 0;
//    while (fgets(buffer, sizeof(buffer), file) != NULL) {
//        // Remove the newline character from the buffer
//        *(buffer+strcspn(buffer, "\n")) = '\0';
//        strcpy(*(dict_words + word_count), buffer);
//        word_count++;
//    }
//
//    // Close the file
//    fclose(file);
//    srand(seed);
//    // find the right word
//
//    /* Create the listener socket as TCP socket (SOCK_STREAM) */
//    int listener = socket( AF_INET, SOCK_STREAM, 0 );
//    /* here, the listener is a socket descriptor (part of the fd table) */
//
//    if ( listener == -1 ) { perror( "socket() failed" ); return EXIT_FAILURE; }
//
//    /* populate the socket structure for bind() */
//    struct sockaddr_in tcp_server;
//    tcp_server.sin_family = AF_INET;  /* IPv4 */
//
//    tcp_server.sin_addr.s_addr = htonl( INADDR_ANY );
//    /* allow any IP address to connect */
//
//    tcp_server.sin_port = htons( listener_port );
//
//    if ( bind( listener, (struct sockaddr *)&tcp_server, sizeof( tcp_server ) ) == -1 )
//    {
//        perror( "bind() failed" );
//        return EXIT_FAILURE;
//    }
//
//    /* identify our port number as a TCP listener port */
//    if ( listen( listener, 5 ) == -1 )
//    {
//        perror( "listen() failed" );
//        return EXIT_FAILURE;
//    }
//    printf( "MAIN: Wordle server listening on port {%d}\n", listener_port );
//    threads = calloc(1000000, sizeof(pthread_t));
//    block_on_accept_msg_t * blockOnAcceptMsg = calloc(1, sizeof(block_on_accept_msg_t));
//    blockOnAcceptMsg->num_words = num_words; blockOnAcceptMsg->dict_words = dict_words;blockOnAcceptMsg->listener = listener;
//    pthread_create(&tid, NULL, block_on_accept, blockOnAcceptMsg);
//    while (break_out == 0);
////    block_on_accept(blockOnAcceptMsg);
//    // Free the allocated memory
////    for (int i = 0; i < MAX_WORDS; i++) {
////        free(*(words + i));
////    }
////    free(words);
////    close( listener );
//
//    return EXIT_SUCCESS;
//}
//
//void * block_on_accept(void * arg) {
//    while (1)
//    {
//        int num_words = ((block_on_accept_msg_t *)arg)->num_words;
//        char ** dict_words = ((block_on_accept_msg_t *)arg)->dict_words;
//        int listener = ((block_on_accept_msg_t *)arg)->listener;
//        int right_word_index = rand() % num_words;
//        char * right_word = *(dict_words + right_word_index);
//        struct sockaddr_in remote_client;
//        int addrlen = sizeof( remote_client );
//        int newsd = accept( listener, (struct sockaddr *)&remote_client,
//                            (socklen_t *)&addrlen );
//        if ( newsd == -1 ) { perror( "accept() failed" ); continue; }
//
//        printf( "MAIN: rcvd incoming connection request\n");
//        wordle_msg_t * msg = calloc(1, sizeof( wordle_msg_t ) );
//        msg->newsd = newsd;msg->right_word = right_word;msg->tid = (pthread_t)(threads+next_thread_id);msg->dict_words = dict_words;msg->num_words = num_words;
//        int rc = pthread_create((threads+next_thread_id), NULL, wordle_one_thread, msg);
//        next_thread_id++;
//        if ( rc != 0 ) { fprintf( stderr, "pthread_create() failed (%d)\n", rc ); break; }
//        //single thread test version
////        wordle_one_thread(msg);
//        record_word_count++;
//        //record the right word in words using realloc
//        words = realloc(words, (record_word_count+1) * sizeof(char *));
//        *(words+record_word_count-1) = calloc(MAX_WORD_LENGTH, sizeof(char));
//        char * right_word_print = calloc(5, sizeof(char));
//        for (int i = 0; i<5; i++) *(right_word_print+i) = toupper(*(right_word+i));
//        strcpy(*(words+record_word_count-1), right_word_print);
//    }
//    return NULL;
//}
//
//void * wordle_one_thread(void * arg) {
//    wordle_msg_t * msg = (wordle_msg_t *) arg;
//    int newsd = msg->newsd;
//    char * right_word = msg->right_word;
//    pthread_t tid = msg->tid;
//    char ** dict_words = msg->dict_words;
//    int num_words = msg->num_words;
//    int n;
//    short * time = calloc(1, sizeof(short));
//    *time = 6;
//    char * result = calloc(8, sizeof(char));
//    do
//    {
//        char * buffer = calloc(MAXBUFFER, sizeof(char));
//
//        printf( "THREAD %ld: waiting for guess\n",tid );    /* or we can use read() */
//
//        n = recv( newsd, buffer, MAXBUFFER, 0 );
//        /* recv() call will block until we receive data (n > 0)
//         *  or an error occurs (n == -1)
//         *   or the client closed its socket and no data rcvd (n == 0)
//         */
//        if ( n == -1 )
//        {
//            perror( "recv() failed" );
////                return EXIT_FAILURE;
//        }
//        if (n == 0)
//        {
//            pthread_mutex_lock(&total_losses_lock);
//            {
//                total_losses++;
//            }
//            pthread_mutex_unlock(&total_losses_lock);
//            printf( " client gave up; closing TCP connection...\n");
//            break;
//        }
//        else /* n > 0 */
//        {
//            if (strchr(buffer, '\n') != NULL)
//            {
//                *(buffer+(n-1)) = '\0';
//            }
//            else
//            {
//                *(buffer+n) = '\0';
//            }
//            for (int i = 0; i<5; i++) *(buffer+i) = tolower(*(buffer+i));
//            printf( "THREAD %ld: rcvd guess: %s\n", tid,buffer);
//            //if the length of buffer is not 5, mark it as invalid guess
//            if (strlen(buffer) != 5) {
//                printf("THREAD %ld: invalid guess; sending reply: ????? (%hd guesses left)\n",tid,*time);
//                *(char *)(result) = 'N';
//                *(unsigned short *)(result+1) = htons( *time );
//                *(char *)(result + 3) = '?';
//                *(char *)(result + 4) = '?';
//                *(char *)(result + 5) = '?';
//                *(char *)(result + 6) = '?';
//                *(char *)(result + 7) = '?';
//                n = send( newsd, result, 8, 0 );
////                    if ( n == -1 ) { perror( "send() failed\n" ); return EXIT_FAILURE; }
//                continue;
//            }
//            //valid guess
//            else {
//                int flag = 0;
//                //if buffer is not in the dict_words, mark it as invalid guess
//                for (int i = 0;i<num_words;i++)
//                {
//                    if (strcmp(buffer, *(dict_words+i)) == 0) {
//                        flag = 1;
//                        break;
//                    }
//                }
//                if (flag == 0)
//                {
//                    //mark as invalid guess
//                    printf("THREAD %ld: invalid guess; sending reply: ????? (%hd guesses left)\n",tid,*time);
//                    *(char *)(result) = 'N';
//                    *(unsigned short *)(result+1) = htons( *time );
//                    *(char *)(result + 3) = '?';
//                    *(char *)(result + 4) = '?';
//                    *(char *)(result + 5) = '?';
//                    *(char *)(result + 6) = '?';
//                    *(char *)(result + 7) = '?';
//                    n = send( newsd, result, 8, 0 );
//                }
//                else {
//                    char * temp_res = calloc(5, sizeof(char));
//                    *time -= 1;
//                    for (int i = 0; i < 5; i++) {
//                        if (*(buffer + i) == *(right_word + i)) {
//                            *(temp_res + i) = *(buffer + i) - 32;
//                        } else if (strchr(right_word, *(buffer + i)) != NULL) {
//                            *(temp_res + i) = *(buffer + i);
//                        } else {
//                            *(temp_res + i) = '-';
//                        }
//                    }
//                    if (*time != 1)
//                    {
//                        printf("THREAD %ld: sending reply: %s (%hd guesses left)\n", tid,temp_res, *time);
//                    }
//                    else{
//                        printf("THREAD %ld: sending reply: %s (%hd guess left)\n", tid,temp_res, *time);
//                    }
//                    *(char *)(result) = 'Y';
//                    *(unsigned short *)(result+1) = htons( *time );
//                    *(char *)(result + 3) = *(temp_res);
//                    *(char *)(result + 4) = *(temp_res + 1);
//                    *(char *)(result + 5) = *(temp_res + 2);
//                    *(char *)(result + 6) = *(temp_res + 3);
//                    *(char *)(result + 7) = *(temp_res + 4);
//                    n = send( newsd, result, 8, 0 );
//                    pthread_mutex_lock(&total_guesses_lock);
//                    {
//                        total_guesses++;
//                    }
//                    pthread_mutex_unlock(&total_guesses_lock);
//                    if (strcmp(buffer, right_word) == 0) {
//                        pthread_mutex_lock(&total_wins_lock);
//                        {
//                            total_wins++;
//                        }
//                        pthread_mutex_unlock(&total_wins_lock);
//                        break;
//                    }
//                    if (*time == 0) {
//                        pthread_mutex_lock(&total_losses_lock);
//                        {
//                            total_losses++;
//                        }
//                        pthread_mutex_unlock(&total_losses_lock);
//                        break;
//                    }
//                }
//            }
//        }
//    }
//    while ( time > 0 );
//    //make the right_word uppercase
//    char * right_word_print = calloc(5, sizeof(char));
//    for (int i = 0; i<5; i++) *(right_word_print+i) = toupper(*(right_word+i));
//    printf("THREAD %ld: game over; word was %s!\n",tid,right_word_print);
//    close( newsd );
//    //free the allocated memory
//    free(time);
//    return NULL;
//}
