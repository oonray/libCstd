#include "ca_buffer.h"
#include <buffer/ca_buffer.h>
#include <errno.h>
#include <string.h>

int ca_buffer_new(ca_buffer **out, size_t mlen, ca_buffer_option opt){
    errno = EINVAL;
    if(out==NULL)   ca_err_throw();
    if(opt<0)       ca_err_throw();
    if(mlen<=0)     ca_err_throw();
    if(*out!=NULL)  ca_err_throw_c(EADDRINUSE);
    ca_err_reset();

    *out = calloc(1,sizeof(ca_buffer));
    if(*out==NULL)ca_err_throw_c(ENOMEM);

    (*out)->opt = opt;
    (*out)->dlen = 0;
    (*out)->mlen = mlen;
    (*out)->data = calloc(1,(*out)->mlen);
    if((*out)->data==NULL)ca_err_throw_c(ENOMEM);

    return 1;
error:
    return 0;
}

int ca_buffer_destroy(ca_buffer **out){
    errno = EINVAL;
    if(out==NULL) ca_err_throw();
    if(*out==NULL) ca_err_throw();
    ca_err_reset();

    if((*out)->data!=NULL){
        memset((*out)->data,0,(*out)->mlen);
        free((*out)->data);
        (*out)->data=NULL;
    }
    memset(*out,0,sizeof(ca_buffer));
    free(*out);
    *out=NULL;

    return 1;
error:
    return 0;
}

int ca_buffer_resize(ca_buffer *buff,size_t mem){
    errno = EINVAL;
    if(buff==NULL) ca_err_throw();
    if(mem<=0) ca_err_throw();
    ca_err_reset();

    void *tmpbuff = realloc(buff->data,mem);
    if(tmpbuff==NULL) ca_err_throw();

    buff->data = tmpbuff;
    buff->mlen = mem;

    return 1;
error:
    return 0;

}

int ca_buffer_write(ca_buffer *buff, void *value, size_t value_len)
{
    errno = EINVAL;
    if(buff==NULL)        ca_err_throw();
    if(value==NULL)       ca_err_throw();
    if(value_len <= 0)    ca_err_throw();
    if(buff==value)       ca_err_throw();
    if(buff->data==NULL)  ca_err_throw();
    if(buff->data==value) ca_err_throw();

    if(buff->opt==CA_BUFFER_NOEXPAND){
        errno = ENOBUFS;
        if(ca_buffer_full(buff))            ca_err_throw();
        if(ca_buffer_space(buff)<value_len) ca_err_throw();
    }
    ca_err_reset();

    if(buff->opt==CA_BUFFER_EXPAND){
        if(value_len>buff->mlen){
            if(!ca_buffer_resize(buff,value_len)) ca_err_throw();
        }
    }

    memcpy(buff->data,value,value_len);
    buff->dlen=value_len;
    return 1;
error:
    return 0;
}

int ca_buffer_write_end(ca_buffer *buff, void *value, size_t value_len)
{
    errno = EINVAL;
    if(buff==NULL)        ca_err_throw();
    if(value==NULL)       ca_err_throw();
    if(value_len <= 0)    ca_err_throw();
    if(buff==value)       ca_err_throw();
    if(buff->data==NULL)  ca_err_throw();
    if(buff->data==value) ca_err_throw();

    if(buff->opt==CA_BUFFER_NOEXPAND){
        errno = ENOBUFS;
        if(ca_buffer_full(buff))            ca_err_throw();
        if(ca_buffer_space(buff)<value_len) ca_err_throw();
    }
    ca_err_reset();

    if(buff->opt==CA_BUFFER_EXPAND){
        if((buff->dlen+value_len)>buff->mlen){
            if(!ca_buffer_resize(buff,buff->dlen+value_len)) ca_err_throw();
        }
    }

    memcpy(buff->data+buff->dlen,value,value_len);
    buff->dlen+=value_len;
    return 1;
error:
    return 0;

}

int ca_buffer_read(ca_buffer *buff, void *out, size_t read_size)
{
    errno = EINVAL;
    if(buff==NULL)        ca_err_throw();
    if(buff->data==NULL)  ca_err_throw();
    if(out==NULL)         ca_err_throw();
    if(read_size <= 0)    ca_err_throw();
    errno = ENOBUFS;
    if(ca_buffer_data(buff)<=0)         ca_err_throw();
    if(ca_buffer_data(buff)<read_size)  ca_err_throw();
    ca_err_reset();

    memcpy(out,buff->data,read_size);

    return 1;
error:
    return 0;
}

int ca_buffer_read_alloc(ca_buffer *buff, void *out, size_t read_size)
{
    errno = EINVAL;
    if(buff==NULL)        ca_err_throw();
    if(buff->data==NULL)  ca_err_throw();
    if(out==NULL)         ca_err_throw();
    if(read_size <= 0)    ca_err_throw();
    errno = ENOBUFS;
    if(ca_buffer_data(buff)<=0)         ca_err_throw();
    if(ca_buffer_data(buff)<read_size)  ca_err_throw();
    ca_err_reset();

    out=calloc(1,read_size);
    if(out==NULL) ca_err_throw();

    memcpy(out,buff->data,read_size);

    return 1;
error:
    return 0;
}

int ca_buffer_read_alloc_all(ca_buffer *buff, void *out)
{
    return ca_buffer_read(buff,out,buff->mlen);
}

