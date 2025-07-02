#include <buffer/ca_buffer.h>
#include <errno.h>
#include <string.h>

int _ca_buffer_new(ca_buffer **out, size_t init_len_mem, size_t type){
    if(out==NULL){errno = EINVAL;goto error;}
    if(*out!=NULL){errno = EADDRINUSE;goto error;}

    *out = calloc(1,sizeof(ca_buffer));
    if(*out==NULL){ errno=ENOMEM; goto error;}

    (*out)->len = 0;
    (*out)->type = type;
    (*out)->mlen = init_len_mem;
    (*out)->buff = calloc(1,init_len_mem);
    if((*out)->buff==NULL){ errno=ENOMEM; goto error;}

    return 1;
error:
    return 0;
}

int ca_buffer_new(ca_buffer **out){
    return ca_buffer_new_t(out,ca_buffer_def_mem,ca_buffer_def_type);
}

int ca_buffer_new_m(ca_buffer **out,size_t memory){
    errno = EINVAL;
    if(memory <= 0) goto error;
    errno =0;

    return _ca_buffer_new(out,memory,ca_buffer_def_type);
error:
    return 0;
}

int ca_buffer_new_t(ca_buffer **out,int amount, size_t type){
    errno = EINVAL;
    if(amount <= 0) goto error;
    if(type <= 0) goto error;
    errno =0;

    return _ca_buffer_new(out,amount*type,type);
error:
    return 0;
}

int ca_buffer_destroy(ca_buffer **out){
    errno = EINVAL;
    if(out==NULL) goto error;
    if(*out==NULL) goto error;
    errno = 0;

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
    if(buff==NULL) goto error;
    if(mem==0) goto error;
    errno = 0;

    void *tmpbuff = realloc(buff->buff,mem);
    if(tmpbuff==NULL) goto error;

    buff->buff = tmpbuff;
    buff->mlen = mem;

    return 1;
error:
    return 0;

}

int ca_buffer_resize_t(ca_buffer *buff,int amount){
    errno = EINVAL;
    if(buff==NULL) goto error;
    if(amount==0) goto error;
    errno = ENOBUFS;

    return ca_buffer_resize(buff,amount*buff->type);
error:
    return 0;
}

int _ca_buffer_write(ca_buffer *buff, size_t start_mem,
                     void *value, size_t value_mem_len,int expand)
{
    errno = EINVAL;
    if(buff==NULL) goto error;
    if(value==NULL) goto error;
    if(start_mem<0) goto error;
    if(value_mem_len <= 0) goto error;
    if(buff==value) goto error;
    if(buff->buff==NULL) goto error;
    if(buff->buff==value) goto error;
    if(!expand){
        errno = ENOBUFS;
        if((start_mem+value_mem_len)>buff->mlen) goto error;
        if(ca_buffer_full(buff)) goto error;
        if(ca_buffer_space(buff)<value_mem_len) goto error;
    }
    errno = 0;

    if(expand){
        if((start_mem+value_mem_len)>buff->mlen){
            if(!ca_buffer_resize(buff,start_mem+value_mem_len)) goto error;
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
    if(buff==NULL) goto error;
    if((N*buff->type)>value_len) goto error;
    errno=0;
    return _ca_buffer_write(buff,0,value,(N*buff->type),0);
error:
    return 0;
}

int ca_buffer_write_ext(ca_buffer *buff, void *value, size_t value_len,int N)
{
    errno = EINVAL;
    if(buff==NULL) goto error;
    if((N*buff->type)>value_len) goto error;
    errno=0;
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
    if(buff==NULL){ errno=EINVAL; goto error;}
    return _ca_buffer_write(buff,buff->len,value,value_len,0);
error:
    return 0;
}

int ca_buffer_write_chunk_ex(ca_buffer *buff,void *value,size_t value_len)
{
    if(buff==NULL){ errno=EINVAL; goto error;}
    return _ca_buffer_write(buff,buff->len,value,value_len,1);
error:
    return 0;
}

int _ca_buffer_read(ca_buffer *buff,size_t start_mem,
                    void **out,size_t m_read_size, int reduce)
{
    errno = EINVAL;
    if(buff==NULL) goto error;
    if(out==NULL) goto error;
    if(*out!=NULL) goto error;
    if(start_mem<0) goto error;
    if(m_read_size <= 0) goto error;
    if(buff->buff==NULL) goto error;
    errno = ENOBUFS;
    if(ca_buffer_data(buff)>0) goto error;
    if((ca_buffer_data(buff)-start_mem)<m_read_size) goto error;
    errno = 0;

    (*out) = calloc(1,m_read_size);
    if(out==NULL) goto error;

    memcpy(out,buff->buff+start_mem,m_read_size);
    if(reduce){
        if(!ca_buffer_resize(buff,buff->mlen-m_read_size)) goto error;
    }

    return 1;
error:
    return 0;
}

int ca_buffer_read_t(ca_buffer *buff, void **out,int amount)
{
    if(buff==NULL){errno=EINVAL; goto error;}
    return _ca_buffer_read(buff,0,out,amount*buff->type,0);
error:
    return 0;
}

int ca_buffer_read_st(ca_buffer *buff,int start, void **out,int amount)
{
    if(buff==NULL){errno=EINVAL; goto error;}
    return _ca_buffer_read(buff,start*buff->type,out,amount*buff->type,0);
error:
    return 0;
}
