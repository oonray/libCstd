#include "generic.h"
#include <stdio.h>

#define TYPE(type) \
int read_##type(buffer_##type *buff,type *out){\
    printf("Reading %s",#type); \
    return 1; \
}
TYPE_LIST
#undef TYPE

#define TYPE(type) \
int write_##type(buffer_##type *buff,type *in){\
    printf("Writing %s",#type); \
    return 1; \
}
TYPE_LIST
#undef TYPE
