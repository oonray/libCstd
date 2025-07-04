#include "ca_error.h"
#include <munit/munit.h>
#include <bstring/bstrlib.h>

#include <buffer/ca_buffer.h>

static void *buffer_tests_setup(const MunitParameter params[], void *user_data) {
  ca_buffer *buff = NULL;
  if(!ca_buffer_new(&buff)) goto error;
  if(buff==NULL) goto error;
  if(buff->mlen != (ca_buffer_def_mem)) ca_err_throw();
  if(buff->type != (ca_buffer_def_type)) ca_err_throw();

  return buff;
error:
  return NULL;
}

static void buffer_test_teardown(void *fixture) {
    if(fixture==NULL) return;
    ca_buffer_destroy((ca_buffer **)&fixture);
}

MunitResult buffer_test_new(const MunitParameter params[],
                     void *user_data_or_fixture) {
  ca_buffer *buff = NULL;
  if(!ca_buffer_new(&buff)) goto error;
  if(buff==NULL) goto error;
  if(buff->mlen != (ca_buffer_def_mem)) ca_err_throw();
  if(buff->type != (ca_buffer_def_type)) ca_err_throw();

  ca_buffer_destroy(&buff);
  if(buff!=NULL) goto error;

  if(!ca_buffer_new_m(&buff,128)) goto error;
  if(buff==NULL) goto error;
  if(buff->mlen!=128) ca_err_throw();
  if(buff->type != (ca_buffer_def_type)) ca_err_throw();

  if(!ca_buffer_destroy(&buff)) goto error;
  if(buff!=NULL) goto error;

  if(!ca_buffer_new_t(&buff,2,sizeof(ca_buffer))) goto error;
  if(buff->mlen!=(2*sizeof(ca_buffer))) ca_err_throw();
  if(buff->type != sizeof(ca_buffer)) ca_err_throw();

  ca_buffer_destroy(&buff);
  if(buff!=NULL) goto error;

  return MUNIT_OK;
error:
  ca_err_print_test();
  return MUNIT_FAIL;
}

MunitResult buffer_test_write(const MunitParameter params[],
                       void *user_data_or_fixture) {
  ca_buffer *buff = (ca_buffer *)user_data_or_fixture;
  if(buff==NULL) return MUNIT_FAIL;

  bstring input = bfromcstr("Hello World");
  bstring input2 = calloc(1,sizeof(struct tagbstring));
  if(input2==NULL) goto error;
  input2->mlen=buff->mlen+input->mlen;
  input2->slen=input2->mlen/sizeof(char);
  input2->data=calloc(1,input2->mlen);
  if(input2->data==NULL) goto error;
  memset(input2->data,'A',input->mlen);

  if(!ca_buffer_write(buff,input->data,input->mlen)) goto error;
  if(memcmp(input->data,buff->buff,input->mlen)!=0) goto error;

  memset(buff->buff,0,buff->mlen);

  if(!ca_buffer_write_t(buff,input->data,input->mlen,2)) goto error;
  if(memcmp(input->data,buff->buff,(2*buff->type))!=0) ca_err_throw();

  memset(buff->buff,0,buff->mlen);

  if(!ca_buffer_write_s(buff,10,input->data,input->mlen)) goto error;
  void *zeroes = calloc(10,buff->type);
  if(zeroes==NULL) goto error;
  if(memcmp(zeroes,buff->buff,(10*buff->type))!=0) ca_err_throw();
  if(memcmp(input->data,buff->buff+(10*buff->type),input->mlen)!=0) ca_err_throw();
  free(zeroes);

  buff->len=buff->mlen;
  if(!ca_buffer_full(buff)) ca_err_throw();
  if(ca_buffer_data(buff)!=buff->mlen) ca_err_throw();
  if(ca_buffer_space(buff)!=0) ca_err_throw();

  int len = buff->len;
  int mlen = buff->mlen;
  if(!ca_buffer_write_ex(buff,input2->data,input2->mlen)) goto error;
  if(buff->len!=input2->mlen) ca_err_throw();
  if(buff->mlen<=mlen) ca_err_throw();
  if(buff->mlen!=input2->mlen) ca_err_throw();
  if(memcmp(input2->data,buff->buff,input2->mlen)!=0) ca_err_throw();

  if(!ca_buffer_resize(buff,mlen)) goto error;
  if(buff->mlen!=mlen) ca_err_throw()

  if(!ca_buffer_write_sex(buff,mlen,input2->data,input2->mlen)) goto error;
  if(buff->len!=(mlen+input2->mlen)) ca_err_throw();
  if(buff->mlen<=mlen) ca_err_throw();
  if(memcmp(buff->buff+mlen,input2->data,input2->mlen)!=0) goto error;

  memset(buff->buff,0,buff->mlen);

  if(!ca_buffer_resize(buff,mlen)) goto error;
  if(buff->mlen!=mlen) ca_err_throw();

  if(!ca_buffer_write_ext(buff,input2->data,input2->mlen,input2->mlen/buff->type)) goto error;
  if(memcmp(buff->buff,input2->data,input2->mlen)!=0) ca_err_throw();

  memset(buff->buff,0,buff->mlen);

  if(!ca_buffer_write_ext(buff,input2->data,input2->mlen,input2->mlen/buff->type)) goto error;
  if(memcmp(buff->buff,input2->data,input2->mlen)!=0) ca_err_throw();
  

  bdestroy(input);
  bdestroy(input2);
  return MUNIT_OK;
error:
  if(input!=NULL) bdestroy(input);
  if(input2!=NULL) bdestroy(input2);
  ca_err_print_test();
  return MUNIT_FAIL;
}

MunitResult buffer_test_read(const MunitParameter params[],
                      void *user_data_or_fixture) {
  ca_buffer *buff = (ca_buffer *)user_data_or_fixture;
  if(buff==NULL) goto error;

  bstring input = bfromcstr("Hello World");
  bstring input2 = calloc(1,sizeof(struct tagbstring));
  if(input2==NULL) goto error;
  input2->mlen=buff->mlen+input->mlen;
  input2->slen=input2->mlen/sizeof(char);
  input2->data=calloc(1,input2->mlen);
  if(input2->data==NULL) goto error;
  memset(input2->data,'A',input->mlen);

  bstring out = &(struct tagbstring){0};
  out->mlen=input->mlen;
  out->slen=input->slen;

  if(!ca_buffer_write(buff,input->data,input->mlen)) goto error;
  if(ca_buffer_data(buff)<=0) ca_err_throw();

  if(!ca_buffer_read(buff,(void **)&out->data,out->mlen)) goto error;
  if(out->data==NULL) ca_err_throw();
  if(memcmp(input->data,out->data,out->mlen)!=0) ca_err_throw();

  bdestroy(input);
  bdestroy(input2);
  return MUNIT_OK;
error:
  if(input!=NULL) bdestroy(input);
  if(input2!=NULL) bdestroy(input2);
  ca_err_print_test();
  return MUNIT_FAIL;
}

int main(int argc, char *const *argv) {
  MunitTest buffer_tests[] = {{"/new", buffer_test_new, buffer_tests_setup, buffer_test_teardown,
                        MUNIT_TEST_OPTION_NONE, NULL},
                       {"/write", buffer_test_write, buffer_tests_setup, buffer_test_teardown,
                        MUNIT_TEST_OPTION_NONE, NULL},
                       {"/read\n", buffer_test_read, buffer_tests_setup, buffer_test_teardown,
                        MUNIT_TEST_OPTION_NONE, NULL},
                       {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

  const MunitSuite buffer_suite = {"/buffer_tests", buffer_tests, NULL, 1,
                            MUNIT_SUITE_OPTION_NONE};

  return munit_suite_main(&buffer_suite, NULL, 0, NULL);
}
