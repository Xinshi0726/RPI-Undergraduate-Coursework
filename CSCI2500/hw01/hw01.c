#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void endianness_converter(char* filename, int size_of_type)
{

    // open the file
    FILE* file1 = fopen(filename, "r");
    if (file1 == NULL)
    {
        return;
    }
    // determine size of the file
    fseek(file1, 0L, SEEK_END);
    int file_size = ftell(file1);
    fseek(file1, 0L, SEEK_SET);
    // allocated space to hold contents
    char *f = ( char  *)  malloc ( sizeof (char) * (file_size/size_of_type));
    // read in the file
    fread(f,sizeof (char),file_size/size_of_type,file1);
    // do byte-wise conversion from big-endian to little-endian
    int s = 0;
    int e = file_size/size_of_type;
    for( ; e>s; s++, e--)
    {
        char tmp = f[s];
        f[s] = f[e];
        f[e] = tmp;
    }
    // write a new file as [filename].converted
    FILE* f2 = fopen("a.dat","w");
    fwrite(f,size_of_type, file_size/size_of_type, f2);

}

int main(){
    endianness_converter("C:\\Users\\Micha\\OneDrive - Rensselaer Polytechnic Institute\\CSCI2500\\hw01\\lab02-data.big.dat",4);
    return 0;
}