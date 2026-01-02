#include "ca_error.h"
#include <ringbuffer/ca_ringbuffer.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int ca_ringbuffer_new(ca_ringbuffer **out,size_t size){
  ca_err_chk_par_null(out);
  ca_err_chk_par_not_null_c(*out,EADDRINUSE);

  *out = calloc(1, sizeof(ca_ringbuffer));
  ca_err_chk_null(*out);

  if(!ca_buffer_new(&(*out)->buffer,size,CA_BUFFER_NOEXPAND)) ca_err_throw();
  ca_err_chk_null((*out)->buffer);

  (*out)->start = 0;
  (*out)->end = 0;

  return 1;
error:
  if(errno != EADDRINUSE){
      if((*out)->buffer!=NULL) ca_buffer_destroy(&(*out)->buffer);
      if(*out!=NULL){
            free(*out);
            *out=NULL;
            out=NULL;
      }
  }
  return 0;
}

int ca_ringbuffer_destroy(ca_ringbuffer **r) {
  ca_err_chk_par_null(r);
  ca_err_chk_par_null(*r);

  ca_buffer_destroy(&(*r)->buffer);

  if (*r) {
    memset(*r,0,sizeof(ca_ringbuffer));
    free(*r);
    *r = NULL;
  }

  ca_err_return_bool();
}

int ca_ringbuffer_write(ca_ringbuffer *r,
                        void *data, size_t amount){
  ca_err_chk_par_null(r);
  ca_err_chk_par_null(r->buffer);
  ca_err_chk_par_null(data);
  ca_err_chk_par(amount<=0);

  if (ca_ringbuffer_empty(r)) r->start = r->end = 0;
  ca_err_chk_c(amount > ca_ringbuffer_avaliable_space(r),ENOBUFS);

  void *result = memcpy(ca_ringbuffer_ends_at(r), data, amount);
  ca_err_chk_null_c(result,EINTR);

  ca_ringbuffer_commit_write(r, amount);
  ca_err_return_bool();
}

int ca_ringbuffer_read(ca_ringbuffer *r,
                       void *target, size_t amount)
{
  ca_err_chk_par_null(r);
  ca_err_chk_par_null(r->buffer);
  ca_err_chk_par_null(target);
  ca_err_chk_par(amount<=0);

  ca_err_chk_c(amount > ca_ringbuffer_avaliable_data(r),ENOBUFS);

  void *result = memcpy(target, ca_ringbuffer_starts_at(r), amount);
  ca_err_chk_null_c(result, EINTR);

  if (r->end == r->start) r->start = r->end = 0;

  ca_ringbuffer_commit_read(r, amount);
  ca_err_return_bool();
}
