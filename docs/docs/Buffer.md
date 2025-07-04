# Buffer

A basic buffer that can be fixed or grow if the user needs it to.

### Using buffer
The buffer has Read and Write operations with the option to extend its size. Functions return false on error and set errno.

```c
#include <errno.h>
#include <string.h>
#include <bstring/bstrlib.h>
#include <buffer/ca_buffer.h>

int main(void) {
    ca_buffer *buff = NULL; //The pointer that will hold the buffer
    bstring input = bfromcstr("Hello World");
    char *output = NULL; //output for read

    //new
    if(!ca_buffer_new(&buff)) goto error; // Create the buffer

    //write
    if(!ca_buffer_write(buff,input->data,input->mlen)) goto error;

    //read
    if(!ca_buffer_read_all(buff,&output)) goto error;

    //destroy
    ca_buffer_destroy(&buff);

    return 1;
error:
    //get the errno set by the functions
    printf("Error: (%d) %s",errno,strerror(errno));
    return 0;
}
```

### ca_buffer
The buffer struct contains a pointer to the data, the amount of data written, the size of the buffer and its type.

```c
typedef struct _ca_buffer {
    unsigned char *buff; //pointer to the data
    size_t len;          //the memory length of data written
    size_t mlen;         //the memory length of the buffer
    size_t type;         //the memory length of the buffer type
} ca_buffer;
```

The type is set by taking `sizeof(T))` where the default is `sizeof(unsigned char)`.

Defaults are defined in `ca_buffer.h`.
```c
// ca_buffer.h
#define ca_buffer_def_len 128
#define ca_buffer_def_type sizeof(unsigned char)
#define ca_buffer_def_mem ca_buffer_def_len * ca_buffer_def_type
```

The new functions can be called with default, a memory size or a type.
When zero values are provided defaults are used.
```c
// ca_buffer.c

int ca_buffer_new(ca_buffer **out){
    return ca_buffer_new_t(out,ca_buffer_def_mem,ca_buffer_def_type);
}

int ca_buffer_new_t(ca_buffer **out,int amount, size_t type){
    if(amount <= 0) amount = ca_buffer_def_len;
    if(type <= 0) type = ca_buffer_def_type;

    //an internal function defined in ca_buffer.c
    return _ca_buffer_new(out,amount*type,type);
}

int ca_buffer_new_m(ca_buffer **out,size_t memory){
    if(memory <= 0) memory = ca_buffer_def_len;

    return _ca_buffer_new(out,memory,ca_buffer_def_type);
}
```


```c
int ca_buffer_new(ca_buffer **out);
```

### Functions

```c
int ca_buffer_new(ca_buffer **out){
    return ca_buffer_new_t(out,ca_buffer_def_mem,ca_buffer_def_type);
}

int ca_buffer_new_t(ca_buffer **out,int amount, size_t type){
    if(amount <= 0) amount = ca_buffer_def_len;
    if(type <= 0) type = ca_buffer_def_type;

    //an internal function defined in ca_buffer.c
    return _ca_buffer_new(out,amount*type,type);
}

int ca_buffer_new_m(ca_buffer **out,size_t memory){
    if(memory <= 0) memory = ca_buffer_def_len;

    return _ca_buffer_new(out,memory,ca_buffer_def_type);
}
```


```c
int ca_buffer_new(ca_buffer **out);
```

### Functions

```c
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

```
