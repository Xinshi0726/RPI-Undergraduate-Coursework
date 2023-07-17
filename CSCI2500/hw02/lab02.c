#include <stdio.h>
#include <stdlib.h>


void checkpoint2(char* filename)
{
  // open file
    FILE* file1 = fopen(filename, "r");

    // get size of file
    fseek(file1, 0L, SEEK_END);
    int file_size = ftell(file1);
    fseek(file1, 0L, SEEK_SET);
    // allocate appropriate amount of space for parsing the file as ints
    int *f = ( int  *)  malloc ( sizeof (int) * (file_size/4));
  // read in file
    fread(f,sizeof (int),file_size/4,file1);
  // output parsed contents
    for(int i=0;i<file_size/4;i++){
        printf("Data point #%3d: %d\n",i,f[i]);
    }
  // close file
  fclose(file1);
}

void checkpoint3(char* filename)
{
    // open file
    FILE* file1 = fopen(filename, "r");

    // get size of file
    fseek(file1, 0L, SEEK_END);
    int file_size = ftell(file1);
    fseek(file1, 0L, SEEK_SET);
    // allocate appropriate amount of space for parsing the file as ints
    unsigned long *f = (unsigned long  *)  malloc ( sizeof (unsigned long) * (file_size/8));
    // read in file
    fread(f,sizeof (unsigned long),(file_size/8),file1);
    // output parsed contents
    for(int i=0;i<(file_size/8);i++){
        printf("Data point #%3d: %lu\n",i,f[i]);
    }
    // close file
    fclose(file1);
}


int main()
{
    char filename[256];
    scanf("%s", filename);

    checkpoint2(filename);
    checkpoint3(filename);

    return 0;
}
