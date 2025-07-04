#ifndef __ca_buffer_h_
#define __ca_buffer_h_
//@file ca_buffer.h

#include <stdio.h>
#include <stdlib.h>
#include <error/ca_error.h>

#define ca_buffer_def_len 128
#define ca_buffer_def_type sizeof(unsigned char)
#define ca_buffer_def_mem ca_buffer_def_len * ca_buffer_def_type

typedef struct _ca_buffer {
    unsigned char *buff;
    size_t len;
    size_t mlen;
    size_t type;
} ca_buffer;

/*
 * allocation
 */
int ca_buffer_new(ca_buffer **out);
int ca_buffer_new_m(ca_buffer **out, size_t memory);
int ca_buffer_new_t(ca_buffer **out, int amount, size_t type);

#define ca_buffer_reset(A) (A)->len=0;
#define ca_buffer_full(A) (ca_buffer_space(A)<=0)

#define ca_buffer_data(A) ((A)->len)
#define ca_buffer_data_t(A) ((A)->len/(A)->type)

#define ca_buffer_space(A) ((A)->mlen-(A)->len)
#define ca_buffer_space_t(A) (((A)->mlen-(A)->len)/(A)->type)

#define ca_buffer_mem(A) (A)->mlen
#define ca_buffer_mem_t(A) (A)->mlen/(A)->type

int ca_buffer_destroy(ca_buffer **out);

int ca_buffer_resize(ca_buffer *buff,size_t mem);
int ca_buffer_resize_t(ca_buffer *buff,int N);

/*
 * Write
 */
//write to buff start
int ca_buffer_write(ca_buffer *buff,void *value,size_t value_len);
//wire to buff start n types
int ca_buffer_write_t(ca_buffer *buff,void *value,size_t value_len,int N);
//write to buff start expand
int ca_buffer_write_ex(ca_buffer *buff,void *value,size_t value_len);
//write to buff start expand n types
int ca_buffer_write_ext(ca_buffer *buff,void *value,size_t value_len, int N);

//write from start location
int ca_buffer_write_s(ca_buffer *buff,size_t start,void *value,size_t value_len);
//write from start location and expand
int ca_buffer_write_sex(ca_buffer *buff,size_t start,void *value,size_t value_len);

//write from len
int ca_buffer_write_chunk(ca_buffer *buff,void *value,size_t value_len);
//write from len and expand
int ca_buffer_write_chunk_ex(ca_buffer *buff,void *value,size_t value_len);

/*
 * Read
 * NB.. allocates out
 */
int ca_buffer_read(ca_buffer *buff, void **out,size_t read_size);
//read n types
int ca_buffer_read_t(ca_buffer *buff, void **out,int N);

// read from start
int ca_buffer_read_s(ca_buffer *buff,size_t start, void **out,size_t read_size);
// read from start n types
int ca_buffer_read_st(ca_buffer *buff,int index, void **out,int N);

int ca_buffer_read_all(ca_buffer *buff,void **out);
#endif
