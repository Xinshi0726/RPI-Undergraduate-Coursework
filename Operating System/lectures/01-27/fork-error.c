//
// Created by Xinshi Wang on 2/10/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
    for (int i = 0; i<3; i++){
        pid_t rc = fork();
        if ( rc == 0 )
        {
            printf("hi\n");
        }
    }

}