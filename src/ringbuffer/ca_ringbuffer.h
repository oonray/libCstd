#ifndef __ca_ringbuffer_h
#define __ca_ringbuffer_h

#include <bstring/bstrlib.h>

typedef struct ca_ringubffer {
  unsigned char *buffer; //BYTE
  int length;
  int start;
  int end;
} ca_ringbuffer;

int ca_ringbuffer_new(ca_ringbuffer **out,int length);
void ca_ringbuffer_destroy(ca_ringbuffer **r);

int ca_ringbuffer_read(ca_ringbuffer *r, char *target, int amount);
int ca_ringbuffer_read_all(ca_ringbuffer *r,
                           unsigned char *out);

int ca_ringbuffer_write(ca_ringbuffer *r, char *data, int amount);

bstring ca_ringbuffer_gets(ca_ringbuffer *r, int amount);

int ca_ringbuffer_get_bstring(ca_ringbuffer *r,
                                  bstring *out,int amount);

#define ca_ringbuffer_avaliable_data(b)                                        \
  (((b)->end + 1) % (b)->length - (b)->start - 1)

#define ca_ringbuffer_avaliable_space(b) (b->length - (b)->end - 1)

#define ca_ringbuffer_full(b)                                                  \
  (ca_ringbuffer_avaliable_data((b)) - (b)->length == 0)

#define ca_ringbuffer_empty(b) (ca_ringbuffer_avaliable_data((b)) == 0)

#define ca_ringbuffer_starts_at(b) ((b)->buffer + (b)->start)

#define ca_ringbuffer_ends_at(b) ((b)->buffer + (b)->end)

#define ca_ringbuffer_commit_read(b, a)                                        \
  ((b)->start = ((b)->start + (a)) % (b)->length)

#define ca_ringbuffer_commit_write(b, a)                                       \
  ((b)->end = ((b)->end + (a)) % (b)->length)

#endif
