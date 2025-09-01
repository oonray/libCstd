#include "ca_buffer.h"
#include "ca_error.h"
#include <buffer/ca_buffer.h>
#include <errno.h>
#include <string.h>

int ca_buffer_new(ca_buffer **out, size_t mlen, ca_buffer_option opt){
    ca_err_chk_par_null(out);
    ca_err_chk_par_not_null(*out);
    ca_err_chk_par(opt<0);
    ca_err_chk_par(mlen<=0);

    *out = calloc(1,sizeof(ca_buffer));
    ca_err_chk_null(*out);

    (*out)->opt = opt;
    (*out)->dlen = 0;
    (*out)->mlen = mlen;
    (*out)->data = calloc(1,(*out)->mlen);
    ca_err_chk_null((*out)->data);

    ca_err_return_bool();
}

int ca_buffer_destroy(ca_buffer **out){
    ca_err_chk_null(out);
    ca_err_chk_null(*out);

    if((*out)->data!=NULL){
        memset((*out)->data,0,(*out)->mlen);
        free((*out)->data);
        (*out)->data=NULL;
    }
    memset(*out,0,sizeof(ca_buffer));
    free(*out);
    *out=NULL;

    ca_err_return_bool();
}

int ca_buffer_resize(ca_buffer *buff,size_t mem){
    ca_err_chk_par_null(buff);
    ca_err_chk_par(mem<=0);

    void *tmpbuff = realloc(buff->data,mem);
    ca_err_chk_null(tmpbuff);

    buff->data = tmpbuff;
    buff->mlen = mem;

    ca_err_return_bool();
}

int ca_buffer_write(ca_buffer *buff, void *value, size_t value_len)
{
    ca_err_chk_par_null(buff);
    ca_err_chk_par_null(value);
    ca_err_chk_par(value_len<=0);
    ca_err_chk_par(buff==value);
    ca_err_chk_par(buff->data==NULL);
    ca_err_chk_par(buff->data==value);

    if(buff->opt==CA_BUFFER_NOEXPAND){
        ca_err_chk_c(ca_buffer_full(buff),ENOBUFS);
        ca_err_chk_c(ca_buffer_space(buff)<value_len,ENOBUFS);
    }

    if(buff->opt==CA_BUFFER_EXPAND){
        if(value_len>buff->mlen){
            ca_err_chk(!ca_buffer_resize(buff,value_len));
        }
    }

    memcpy(buff->data,value,value_len);
    buff->dlen=value_len;
    ca_err_return_bool();
}

int ca_buffer_write_loc(ca_buffer *buff, size_t start, void *value, size_t value_len)
{
    ca_err_chk_par_null(buff);
    ca_err_chk_par_null(value);
    ca_err_chk_par(start>=0);
    ca_err_chk_par(start<buff->mlen);
    ca_err_chk_par(value_len<=0);
    ca_err_chk_par(buff==value);
    ca_err_chk_par(buff->data==NULL);
    ca_err_chk_par(buff->data==value);

    if(buff->opt==CA_BUFFER_NOEXPAND){
        ca_err_chk_c(ca_buffer_full(buff),ENOBUFS);
        ca_err_chk_c(ca_buffer_space(buff)<value_len,ENOBUFS);
    }

    if(buff->opt==CA_BUFFER_EXPAND){
        if(value_len>buff->mlen){
            ca_err_chk(!ca_buffer_resize(buff,value_len));
        }
    }

    memcpy(buff->data,value,value_len);
    buff->dlen=value_len;
    ca_err_return_bool();
}

int ca_buffer_write_end(ca_buffer *buff,
                        void *value, size_t value_len)
{
    ca_err_chk_par_null(buff);
    ca_err_chk_par_null(buff->data);
    ca_err_chk_par_null(value);
    ca_err_chk_par(value_len<=0);
    ca_err_chk_par(buff==value);
    ca_err_chk_par(buff->data==value);

    if(buff->opt==CA_BUFFER_NOEXPAND){
        ca_err_chk_c(ca_buffer_full(buff),ENOBUFS);
        ca_err_chk_c(ca_buffer_space(buff)<value_len,ENOBUFS);
    }

    if(buff->opt==CA_BUFFER_EXPAND){
        if((buff->dlen+value_len)>buff->mlen){
            if(!ca_buffer_resize(buff,buff->dlen+value_len)) ca_err_throw();
        }
    }

    memcpy(buff->data+buff->dlen,value,value_len);
    buff->dlen+=value_len;
    ca_err_return_bool();
}

int ca_buffer_read(ca_buffer *buff, void *out, size_t read_size)
{
    ca_err_chk_par_null(buff);
    ca_err_chk_par_null(buff->data);
    ca_err_chk_par_null(out);
    ca_err_chk_par(read_size <= 0);
    ca_err_chk_par(read_size > buff->mlen);

    ca_err_chk_c(ca_buffer_data(buff)<=0,ENOBUFS);
    ca_err_chk_c(ca_buffer_data(buff)<read_size,ENOBUFS);

    memcpy(out,buff->data,read_size);
    ca_err_return_bool();
}

int ca_buffer_read_loc(ca_buffer *buff, size_t start, void *out, size_t read_size)
{
    ca_err_chk_par_null(buff);
    ca_err_chk_par_null(buff->data);
    ca_err_chk_par_null(out);
    ca_err_chk_par(start>=0);
    ca_err_chk_par(start<buff->mlen);
    ca_err_chk_par(read_size <= 0);
    ca_err_chk_par(read_size > buff->mlen);

    ca_err_chk_c(ca_buffer_data(buff)<=0,ENOBUFS);
    ca_err_chk_c(ca_buffer_data(buff)<read_size,ENOBUFS);

    memcpy(out,buff->data,read_size);
    ca_err_return_bool();
}

int ca_buffer_read_alloc(ca_buffer *buff,
                         void **out, size_t read_size)
{
    ca_err_chk_par_null(buff);
    ca_err_chk_par_null(buff->data);
    ca_err_chk_par_null(out);
    ca_err_chk_par(read_size<=0);

    ca_err_chk_c(ca_buffer_data(buff)<=0,ENOBUFS);
    ca_err_chk_c(ca_buffer_data(buff)<read_size,ENOBUFS);

    *out=calloc(1,read_size);
    ca_err_chk_null(out);

    if(!ca_buffer_read(buff,*out,read_size)){
        free(*out);
        *out=NULL;
        ca_err();
    }

    ca_err_return_bool();
}

int ca_buffer_read_alloc_all(ca_buffer *buff,
                             void **out,size_t *amount_read)
{
    *amount_read=buff->dlen;
    return ca_buffer_read_alloc(buff,out,buff->dlen);
}

