#ifndef __ca_io_h
#define __ca_io_h

#include <error/ca_error.h>
#include <bstring/bstrlib.h>
#include <ca_ringbuffer.h>

#define CA_IO_NL "\n"
#define CA_IO_CRLF "\n\r"

#define _CA_IO_TYPE \
 _CA_IO_TYPE_X(CA_SOCKFD,0x01,recv,send) \
 _CA_IO_TYPE_X(CA_FILEFD,0x03,read,write) \
 _CA_IO_TYPE_X(CA_STRINGFD,0x05,read,write) \
 _CA_IO_TYPE_X(CA_PIPEFD,0x09,read,write) \
 _CA_IO_TYPE_X(CA_SSLFD,0x11,recv,send) \
 _CA_IO_TYPE_X(CA_SERIAL,0x13,read,send) \
 _CA_IO_TYPE_X(CA_BUFFER,0xA1,read,send) \

const static struct tagbstring CA_IO_NL_BS = bsStatic(CA_IO_NL);
const static struct tagbstring CA_IO_CRLF_BS = bsStatic(CA_IO_CRLF);

#define _CA_IO_TYPE_X(a,b,c,d) a=b,

typedef enum _ca_io_type {
    _CA_IO_TYPE
}ca_io_type;

#undef _CA_IO_TYPE_X
#define _CA_IO_TYPE_X(a,b,c,d) b:c,

#define ca_read(TYPE,TARGET,BUFFER,SIZE) _Generic((TYPE), \
    CA_IO_TYPE \
)
#undef _CA_IO_TYPE_X
#define _CA_IO_TYPE_X(a,b,c,d) b:d,
#define ca_write(TYPE,TARGET,BUFFER,SIZE) _Generic((TYPE), \
    CA_IO_TYPE \
)
#undef _CA_IO_TYPE_X

#endif
