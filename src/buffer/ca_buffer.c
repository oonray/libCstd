#include <buffer/ca_buffer.h>
#include <errno.h>
#include <string.h>

int _ca_buffer_new(ca_buffer **out, size_t init_len_mem, size_t type){
    if(out==NULL)ca_err_throw_c(EINVAL);
    if(*out!=NULL)ca_err_throw_c(EADDRINUSE);

    *out = calloc(1,sizeof(ca_buffer));
    if(*out==NULL)ca_err_throw_c(ENOMEM);

    (*out)->len = 0;
    (*out)->type = type;
    (*out)->mlen = init_len_mem;
    (*out)->buff = calloc(1,init_len_mem);
    if((*out)->buff==NULL)ca_err_throw_c(ENOMEM);

    return 1;
error:
    return 0;
}

int ca_buffer_new(ca_buffer **out){
    return ca_buffer_new_t(out,ca_buffer_def_mem,ca_buffer_def_type);
}

int ca_buffer_new_m(ca_buffer **out,size_t memory){
    if(memory <= 0) memory = ca_buffer_def_len;

    return _ca_buffer_new(out,memory,ca_buffer_def_type);
}

int ca_buffer_new_t(ca_buffer **out,int amount, size_t type){
    if(amount <= 0) amount = ca_buffer_def_len;
    if(type <= 0) type = ca_buffer_def_type;

    return _ca_buffer_new(out,amount*type,type);
}

int ca_buffer_destroy(ca_buffer **out){
    errno = EINVAL;
    if(out==NULL) ca_err_throw();
    if(*out==NULL) ca_err_throw();
    ca_err_reset();

    if((*out)->buff!=NULL){
        memset((*out)->buff,0,(*out)->mlen);
        free((*out)->buff);
        (*out)->buff=NULL;
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
    if(mem==0) ca_err_throw();
    ca_err_reset();

    void *tmpbuff = realloc(buff->buff,mem);
    if(tmpbuff==NULL) ca_err_throw();

    buff->buff = tmpbuff;
    buff->mlen = mem;

    return 1;
error:
    return 0;

}

int ca_buffer_resize_t(ca_buffer *buff,int amount){
    errno = EINVAL;
    if(buff==NULL) ca_err_throw();
    if(amount==0) ca_err_throw();
    errno = ENOBUFS;

    return ca_buffer_resize(buff,amount*buff->type);
error:
    return 0;
}

int _ca_buffer_write(ca_buffer *buff, size_t start_mem,
                     void *value, size_t value_mem_len,int expand)
{
    errno = EINVAL;
    if(buff==NULL) ca_err_throw();
    if(value==NULL) ca_err_throw();
    if(start_mem<0) ca_err_throw();
    if(value_mem_len <= 0) ca_err_throw();
    if(buff==value) ca_err_throw();
    if(buff->buff==NULL) ca_err_throw();
    if(buff->buff==value) ca_err_throw();
    if(!expand){
        errno = ENOBUFS;
        if((start_mem+value_mem_len)>buff->mlen) ca_err_throw();
        if(ca_buffer_full(buff)) ca_err_throw();
        if(ca_buffer_space(buff)<value_mem_len) ca_err_throw();
    }
    ca_err_reset();

    if(expand){
        if((start_mem+value_mem_len)>buff->mlen){
            if(!ca_buffer_resize(buff,start_mem+value_mem_len)) ca_err_throw();
        }
    }

    memcpy(buff->buff+start_mem,value,value_mem_len);
    buff->len=start_mem+value_mem_len;
    return 1;
error:
    return 0;
}

int ca_buffer_write(ca_buffer *buff, void *value, size_t value_len)
{
    return _ca_buffer_write(buff,0,value,value_len,0);
}

int ca_buffer_write_t(ca_buffer *buff, void *value, size_t value_len,int N)
{
    errno = EINVAL;
    if(buff==NULL) ca_err_throw();
    if((N*buff->type)>value_len) ca_err_throw();
    ca_err_reset();
    return _ca_buffer_write(buff,0,value,(N*buff->type),0);
error:
    return 0;
}

int ca_buffer_write_ext(ca_buffer *buff, void *value, size_t value_len,int N)
{
    errno = EINVAL;
    if(buff==NULL) ca_err_throw();
    if((N*buff->type)>value_len) ca_err_throw();
    ca_err_reset();
    return _ca_buffer_write(buff,0,value,value_len,1);
error:
    return 0;
}

int ca_buffer_write_ex(ca_buffer *buff, void *value, size_t value_len)
{
    return _ca_buffer_write(buff,0,value,value_len,1);
}

int ca_buffer_write_s(ca_buffer *buff,size_t start,void *value,size_t value_len)
{
    return _ca_buffer_write(buff,start,value,value_len,0);
}

int ca_buffer_write_sex(ca_buffer *buff,size_t start,void *value,size_t value_len)
{
    return _ca_buffer_write(buff,start,value,value_len,1);
}

int ca_buffer_write_chunk(ca_buffer *buff,void *value,size_t value_len)
{
    if(buff==NULL)ca_err_throw_c(EINVAL);
    return _ca_buffer_write(buff,buff->len,value,value_len,0);
error:
    return 0;
}

int ca_buffer_write_chunk_ex(ca_buffer *buff,void *value,size_t value_len)
{
    if(buff==NULL)ca_err_throw_c(EINVAL);
    return _ca_buffer_write(buff,buff->len,value,value_len,1);
error:
    return 0;
}

int _ca_buffer_read(ca_buffer *buff,size_t start_mem,
                    void **out,size_t m_read_size, int reduce)
{
    errno = EINVAL;
    if(buff==NULL) ca_err_throw();
    if(out==NULL) ca_err_throw();
    if(*out!=NULL) ca_err_throw();
    if(start_mem<0) ca_err_throw();
    if(m_read_size <= 0) ca_err_throw();
    if(buff->buff==NULL) ca_err_throw();
    errno = ENOBUFS;
    if(ca_buffer_data(buff)<=0) ca_err_throw();
    if((ca_buffer_data(buff)-start_mem)<m_read_size) ca_err_throw();
    ca_err_reset();

    (*out) = calloc(1,m_read_size);
    if(out==NULL) ca_err_throw();

    memcpy(*out,buff->buff+start_mem,m_read_size);
    if(reduce){
        if(!ca_buffer_resize(buff,buff->mlen-m_read_size)) ca_err_throw();
    }

    return 1;
error:
    return 0;
}

int ca_buffer_read(ca_buffer *buff, void **out,size_t read_size){
    return _ca_buffer_read(buff,0,out,read_size,0);
}

int ca_buffer_read_t(ca_buffer *buff, void **out,int amount)
{
    if(buff==NULL)ca_err_throw_c(EINVAL);
    return _ca_buffer_read(buff,0,out,amount*buff->type,0);
error:
    return 0;
}

int ca_buffer_read_st(ca_buffer *buff,int start, void **out,int amount)
{
    if(buff==NULL)ca_err_throw_c(EINVAL);
    return _ca_buffer_read(buff,start*buff->type,out,amount*buff->type,0);
error:
    return 0;
}
