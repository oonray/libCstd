#include <sockets/soc.h>

int ca_lisener_set(ca_soc *soc,ca_soc_type type, ca_soc_proto proto, bstring host,int port){
    if(soc==NULL){
        errno=EINVAL;
        return 0;
    }

    return 1;
error:
    return 0;
}

int ca_soc_new(ca_soc **out){
    if(out==NULL){ //no input
        errno=EINVAL;
        return 0;
    }
    if((*out)!=NULL){ //already allocated
        errno=EINVAL;
        return 0;
    }
    (*out) = calloc(1,sizeof(ca_soc));
    if(*out==NULL){ //allocation error
        return 0;
    }

    return 1;
error:
    return 0;
}

void ca_soc_destroy(ca_soc *soc){
    if(soc->listener.host!=NULL){
        bdestroy(soc->listener.host);
    }
    free(soc);
}
