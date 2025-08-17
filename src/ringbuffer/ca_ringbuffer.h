#ifndef __ca_ringbuffer_h
#define __ca_ringbuffer_h

#include <error/ca_error.h>
#include <buffer/ca_buffer.h>
#include <bstring/bstrlib.h>

#define CA_RINGBUFFER_DEFAULT_SIZE 128*sizeof(char)

typedef struct ca_ringubffer {
  ca_buffer *buffer;
  int start;
  int end;
} ca_ringbuffer;

int ca_ringbuffer_new(ca_ringbuffer **out,size_t size);
int ca_ringbuffer_destroy(ca_ringbuffer **r);

int ca_ringbuffer_write(ca_ringbuffer *r, void *data,
                        size_t amount);

int ca_ringbuffer_read(ca_ringbuffer *r, void *target,
                       size_t amount);

#define ca_ringbuffer_new_default(ringbuffer) \
ca_ringbuffer_new(ringbuffer,CA_RINGBUFFER_DEFAULT_SIZE)

#define ca_ringbuffer_avaliable_data(b)                                        \
  (((b)->end + 1) % (b)->buffer->mlen - (b)->start - 1)

#define ca_ringbuffer_avaliable_space(b) (b->buffer->mlen - (b)->end - 1)

#define ca_ringbuffer_full(b)                                                  \
  (ca_ringbuffer_avaliable_data((b)) - (b)->buffer->mlen == 0)

#define ca_ringbuffer_empty(b) (ca_ringbuffer_avaliable_data((b)) == 0)

#define ca_ringbuffer_starts_at(b) ((b)->buffer->data + (b)->start)

#define ca_ringbuffer_ends_at(b) ((b)->buffer->data + (b)->end)

#define ca_ringbuffer_commit_read(b, a)                                        \
  ((b)->start = ((b)->start + (a)) % (b)->buffer->mlen)

#define ca_ringbuffer_commit_write(b, a)                                       \
  ((b)->end = ((b)->end + (a)) % (b)->buffer->mlen)

#endif
