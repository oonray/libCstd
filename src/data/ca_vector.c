#include <ca_vector.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int ca_vector_new(ca_vector **out, size_t mlen){
    if(out==NULL
        || *out!=NULL
        || mlen<0){
            errno=EINVAL;
            return false;
        }

    *out = calloc(1,sizeof(ca_vector));
    if(*out==NULL) return false;

    (*out)->dlen = 0;

    (*out)->mlen = mlen;
    if((*out)->mlen<=CA_VECTOR_DEF)
        (*out)->mlen=CA_VECTOR_DEF_SIZE;

    (*out)->data = calloc(1,(*out)->mlen);
    if((*out)->data==NULL) return false;

    return true;
}

int ca_vector_destroy(ca_vector **out){
    if(out==NULL
        || *out==NULL){
            errno=EINVAL;
            return false;
        }

    if((*out)->data!=NULL){
        memset((*out)->data,0,(*out)->mlen);
        free((*out)->data);
        (*out)->data=NULL;
    }

    memset(*out,0,sizeof(ca_vector));
    free(*out);
    *out=NULL;

    return true;
}

int ca_vector_resize(ca_vector *buff,size_t mem){
    if(buff!=NULL
        || mem<=0){
            errno=EINVAL;
            return false;
        }

    void *tmpbuff = realloc(buff->data,mem);
    if(tmpbuff==NULL) return false;

    buff->data = tmpbuff;
    buff->mlen = mem;

    return true;
}

int ca_vector_write(ca_vector *buff, void *value, size_t value_len)
{
    if(buff==NULL
        || value==NULL
        || value_len<=0
        || buff==value
        || buff->data==NULL
        || buff->data==value){
            errno=EINVAL;
            return false;
        }

    if(value_len>buff->mlen){
        if(!ca_vector_resize(buff,value_len)){
            errno=EINVAL;
            return false;
        }
    }

    memcpy(buff->data,value,value_len);
    buff->dlen=value_len;
    return true;
}

int ca_vector_write_loc(ca_vector *buff, size_t start, void *value, size_t value_len)
{
    if(buff==NULL
        || value==NULL
        || value_len<=0
        || start<=0
        || start>=buff->mlen
        || buff->data==NULL
        || buff->data==value){
            errno=EINVAL;
            return false;
        }

    if(value_len>buff->mlen)
        if(!ca_vector_resize(buff,value_len))
            return false;

    memcpy(buff->data,value,value_len);
    buff->dlen=value_len;
    return true;
}

int ca_vector_read(ca_vector *buff, void *out, size_t read_size)
{
    if(buff==NULL
        || out==NULL
        || read_size<=0
        || buff->data==NULL
        || buff->data==out){
            errno=EINVAL;
            return false;
        }

    if(ca_vector_data(buff)<=0){
            errno=ENOBUFS;
            return 0;
        }

    size_t read = read_size;
    if(read_size > buff->mlen)
        read=buff->mlen;

    memcpy(out,buff->data,read);
    return true;
}

int ca_vector_read_loc(ca_vector *buff, size_t start, void *out, size_t read_size)
{
    if(buff==NULL
        || out==NULL
        || read_size<=0
        || start<0
        || start>buff->mlen
        || buff->data==NULL
        || buff->data==out){
            errno=EINVAL;
            return false;
        }

    if(ca_vector_data(buff)<=0){
            errno=ENOBUFS;
            return false;
        }

    size_t read = read_size;
    if(read_size > buff->mlen)
        read=buff->mlen;

    memcpy(out,buff->data+start,read);
    return true;
}
