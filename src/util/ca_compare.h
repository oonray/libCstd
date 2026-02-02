#ifndef __ca_compare_h
#define __ca_compare_h

int (*ca_compare_handler)(void *a, void *b, int len);
int ca_compare(void *a, void *b,int len);

#endif

#ifdef CA_COMPARE_IMPL
#include <stdbool.h>

int ca_compare(void *a, void *b, int len){
    for(int i=0;i<len;i++){
        if(((*(unsigned char *)a)^(*(unsigned char *)b))==0){
            return false;
        }
    }
    return true;
}

#endif

