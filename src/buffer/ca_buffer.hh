#ifndef __ca_buffer_hh
#define __ca_buffer_hh

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <error/ca_error.h>

namespace ca {

typedef enum buffer_default{
    ITEMS=128,
    SIZE=(ITEMS*sizeof(char)),
}buffer_default;

typedef enum buffer_option {
    NOEXPAND=0,
    EXPAND=1,
} buffer_option;

class buffer {
    unsigned char *data;
    buffer_option opt;
    std::size_t dlen;
    std::size_t mlen;
public:
    buffer();
    buffer(std::size_t size);
    buffer(buffer_option opt);
    buffer(std::size_t size,buffer_option opt);
    ~buffer();
    int read(void *out,std::size_t size);
    int write(void *in,std::size_t size);
    int resize(std::size_t size);
    std::size_t size();
    std::size_t left();
    std::size_t avaliable();
    int expandable();
    int configure(buffer_option opt);
    int full();
};

}

#endif
