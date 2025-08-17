#ifndef __ca_buffer_h_
#define __ca_buffer_h_
//@file ca_buffer.h

#include <stdio.h>
#include <stdlib.h>
#include <error/ca_error.h>

#define CA_BUFFER_DEF_ITEMS 128
#define CA_BUFFER_DEF_SIZE (CA_BUFFER_DEF_ITEMS * sizeof(char))

typedef enum _ca_buffer_option {
    CA_BUFFER_NOEXPAND=0,
    CA_BUFFER_EXPAND=1,
} ca_buffer_option;

typedef struct _ca_buffer {
    unsigned char *data;
    ca_buffer_option opt;
    size_t dlen;
    size_t mlen;
} ca_buffer;

int ca_buffer_new(ca_buffer **out, size_t mlen, ca_buffer_option opt);

int ca_buffer_destroy(ca_buffer **out);
int ca_buffer_resize(ca_buffer *buff, size_t mem);

int ca_buffer_write(ca_buffer *buff, void *value, size_t value_len);
int ca_buffer_write_end(ca_buffer *buff, void *value, size_t value_len);

int ca_buffer_read(ca_buffer *buff, void *out, size_t read_size);

int ca_buffer_read_alloc(ca_buffer *buff,
                         void **out, size_t read_size);
int ca_buffer_read_alloc_all(ca_buffer *buff,
                             void **out,size_t *amount_read);

#define ca_buffer_reset(A) (A)->dlen=0;
#define ca_buffer_full(A) (ca_buffer_space(A)<=0)

#define ca_buffer_data(A) ((A)->dlen)
#define ca_buffer_space(A) ((A)->mlen-(A)->dlen)
#define ca_buffer_size(A) (A)->mlen

#define ca_buffer_new_default(OUT) \
ca_buffer_new(OUT,CA_BUFFER_DEF_SIZE,CA_BUFFER_NOEXPAND)

#define ca_buffer_new_default_expand(OUT) \
ca_buffer_new(OUT,CA_BUFFER_DEF_SIZE,CA_BUFFER_EXPAND)

#endif
