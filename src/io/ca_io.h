#ifndef __ca_io_h
#define __ca_io_h

#include <error/ca_error.h>
#include <bstring/bstrlib.h>
#include <ca_ringbuffer.h>

#define CA_IO_NL "\n"
#define CA_IO_CRLF "\n\r"

const static struct tagbstring CA_IO_NL_BS = bsStatic(CA_IO_NL);
const static struct tagbstring CA_IO_CRLF_BS = bsStatic(CA_IO_CRLF);

typedef enum _ca_io_socket_type {
  CA_SOCKFD = 0x01,
  CA_FILEFD = 0x03,
  CA_STRINGFD = 0x05,
  CA_PIPEFD = 0x09,
  CA_SSLFD = 0x11,
  CA_SERIAL = 0x13,
}ca_io_socket_type;

//READ
typedef enum _ca_io_reader_type {
    CA_IO_READER_BUFFER=0x10,
    CA_IO_READER_BUFFER_RING=0x11,
    CA_IO_READER_SOCKET=0x01,
    CA_IO_READER_SOCKET_SSL=0x12,
    CA_IO_READER_SOCKET_SERIAL=0x13,
    CA_IO_READER_FILE=0x03,
    CA_IO_READER_PIPE=0x09,
}ca_io_reader_type;

typedef union _ca_io_readable {
    ca_buffer *buffer;
    ca_ringbuffer *ring_buffer;
} ca_io_readable;

typedef union _ca_io_reader_func {
    ca_error(*ca_buffer_read)(ca_buffer *,void *, size_t);
    ca_error(*ca_ringbuffer_read)(ca_ringbuffer *,void *, size_t);
}ca_io_reader_func;

typedef struct _ca_io_reader {
    ca_io_readable readable;
    ca_io_reader_func reader;
}ca_io_reader;

//WRITE
typedef enum _ca_io_writer_type {
    CA_IO_WRITER_BUFFER,
    CA_IO_WRITER_BUFFER_RING,
    CA_IO_WRITER_SOCKET,
    CA_IO_WRITER_SOCKET_SSL,
    CA_IO_WRITER_FILE,
    CA_IO_WRITER_PIPE,
}ca_io_writer_type;

typedef union _ca_io_writable {
    ca_buffer *buffer;
    ca_ringbuffer *ring_buffer;
} ca_io_writable;

typedef union _ca_io_writer_func {
    ca_error(*ca_buffer_read)(ca_buffer *,void *, size_t);
    ca_error(*ca_ringbuffer_read)(ca_buffer *,void *, size_t);
}ca_io_writer_func;

typedef struct _ca_io_writer {
    ca_io_writable readable;
    ca_io_reader_func write;
}ca_io_writer;

//RW
typedef struct _ca_io_read_writer {
    ca_io_reader *reader;
    ca_io_writer *writer;
}ca_io_read_writer;

#endif
