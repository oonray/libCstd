#ifndef TYPE_LIST 
#define TYPE_LIST \
    TYPE(int) \
    TYPE(char) \
    TYPE(byte)
#endif

#ifndef read
#define TYPE(type) type: ##type: read_##type;
#define read(BUFF,OUT) _Generic((BUFF,OUT), \
        TYPE_LIST \
        )(BUFF,OUT)
#undef TYPE
#endif

#ifndef write
#define TYPE(type) type: ##type: read_##type;
#define write(BUFF,OUT) _Generic((BUFF,OUT), \
        TYPE_LIST \
        )(BUFF,OUT)
#undef TYPE
#endif

#ifndef __generics_test_h_
#define __generics_test_h_

typedef unsigned char byte;

#define TYPE(type) \
typedef struct buffer_##type { \
    type *buffer; \
} buffer_##type;

TYPE_LIST 
#undef TYPE

#define TYPE(type) \
int read_##type(buffer_##type *buff,type *out);
TYPE_LIST
#undef TYPE

#define TYPE(type) \
int write_##type(buffer_##type *buff,type *out);
TYPE_LIST
#undef TYPE



void __attribute__((constructor)) func1();
void __attribute__((destructor)) func2();

#endif
