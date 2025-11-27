#include <stdlib.h>
#include "generic.h"

int main(int argc, char **argv){

    byte *hello = (byte *)"Hello";
    buffer_byte *buff = calloc(1,sizeof(buffer_byte));

    write(buff,hello);
    read(buff,&hello);

    return 0;
}
