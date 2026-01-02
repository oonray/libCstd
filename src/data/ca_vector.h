#ifndef __ca_vector_h_
#define __ca_vector_h_
//@file ca_vector.h

#include <stdio.h>

#define CA_VECTOR_DEF 0
#define CA_VECTOR_DEF_ITEMS 32
#define CA_VECTOR_DEF_SIZE (CA_VECTOR_DEF_ITEMS * sizeof(unsigned char))

typedef struct _ca_vector {
    unsigned char *data;
    size_t dlen;
    size_t mlen;
} ca_vector;

int ca_vector_new(ca_vector **out, size_t mlen);

int ca_vector_destroy(ca_vector **out);
int ca_vector_resize(ca_vector *buff,
                     size_t mem);

//WRITE
int ca_vector_write(ca_vector *buff,
                    void *value,
                    size_t value_len);
int ca_vector_write_loc(ca_vector *buff,
                        size_t start,
                        void *value,
                        size_t value_len);
//READ
int ca_vector_read(ca_vector *buff,
                   void *out,
                   size_t read_size);

int ca_vector_read_loc(ca_vector *buff,
                       size_t start,
                       void *out,
                       size_t read_size);

//MACRO
#define ca_vector_reset(A) (A)->dlen=0;
#define ca_vector_full(A) (ca_vector_space(A)<=0)

#define ca_vector_data(A) ((A)->dlen)
#define ca_vector_space(A) ((A)->mlen-(A)->dlen)
#define ca_vector_size(A) (A)->mlen

#endif
