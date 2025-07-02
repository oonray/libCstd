#include <ca_ringbuffer.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int ca_ringbuffer_new(ca_ringbuffer **out,int length){
  if(out==NULL){ errno=EINVAL; goto error;}
  if(*out!=NULL){ errno=EADDRINUSE; goto error;}

  *out = calloc(1, sizeof(ca_ringbuffer));
  if(*out==NULL) goto error;

  (*out)->buffer = calloc((*out)->length, sizeof(unsigned char));
  if((*out)->buffer==NULL) goto error;

  (*out)->length = length + 1;
  (*out)->start = 0;
  (*out)->end = 0;

  return 1;
error:
  if(errno != EADDRINUSE){ //ADDR in use == ALLOCATED
      if(*out!=NULL){
            free(*out);
            *out=NULL;
            out=NULL;
      }
  }
  return 0;
}

void ca_ringbuffer_destroy(ca_ringbuffer **r) {
  if (*r) {
    if((*r)->buffer) free((*r)->buffer);
    free(*r);
  }
}

int ca_ringbuffer_write(ca_ringbuffer *r, char *data, int length) {
  errno=EINVAL;
  if(r==NULL) goto error;
  if(data==NULL) goto error;
  if(length<=0) goto error;
  errno=0;

  if (ca_ringbuffer_avaliable_data(r) == 0) r->start = r->end = 0;

  if(length > ca_ringbuffer_avaliable_space(r)){
        errno=ENOBUFS;goto error;}

  void *result = memcpy(ca_ringbuffer_ends_at(r), data, length);
  if(result == NULL){errno = EINTR;goto error;}

  ca_ringbuffer_commit_write(r, length);

  return 1;
error:
  return 0;
}

int ca_ringbuffer_read(ca_ringbuffer *r, char *target, int amount) {

  errno=EINVAL;
  if(r==NULL){ goto error;}
  if(target==NULL){goto error;}
  if(amount<=0){goto error;}
  errno=0;

  if(amount > ca_ringbuffer_avaliable_data(r)){
        errno=ENOBUFS;goto error;}

  void *result = memcpy(target, ca_ringbuffer_starts_at(r), amount);
  if(result == NULL){errno = EINTR;goto error;}

  if (r->end == r->start) r->start = r->end = 0;

  ca_ringbuffer_commit_read(r, amount);

  return 1;
error:
  return 0;
}

int ca_ringbuffer_read_commit(ca_ringbuffer *r, int amount) {

    return amount;
error:
    return 0;
}

int ca_ringbuffer_read_all(ca_ringbuffer *r,
                           unsigned char *out){
  if(r==NULL){errno=EFAULT; goto error;}
  if(*out!=NULL){errno=EADDRINUSE; goto error;}
  if(*out!=NULL){errno=EADDRINUSE; goto error;}

  int amount = ca_ringbuffer_avaliable_data(r);
  char *data = calloc(amount, sizeof(char));
  if(data==NULL) goto error;

  int rc = ca_ringbuffer_read(r, data, amount);
  errno=EIO;
  if(rc <= 0) goto error;
  if(rc != amount) goto error;
  if(data == NULL) goto error;
  errno=0;

  *out = bfromcstr(data);
  if(*out==NULL) goto error;

  errno=EIO;
  if(*out==NULL) goto error;
  if(ca_ringbuffer_avaliable_data(r) > 0) goto error;
  errno=0;

  return amount;
error:
  if(errno!=EADDRINUSE) out=NULL;
  return 0;
}

int ca_ringbuffer_get_bstring(ca_ringbuffer *r,
                                  bstring *out,int amount) {
  if(r==NULL){errno=EFAULT; goto error;}
  if(amount<=0){errno=EINVAL; goto error;}
  if(*out!=NULL){errno=EADDRINUSE; goto error;}

  (*out) = blk2bstr(ca_ringbuffer_starts_at(r), amount);
  errno=EIO;
  if(*out==NULL) goto error;
  if(blength(*out) != amount) goto error;

  ca_ringbuffer_commit_read(r, amount);
  if(ca_ringbuffer_avaliable_data(r) < 0) goto error;

  return 1;
error:
  return 0;
}
