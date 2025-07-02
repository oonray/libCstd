#include <buffer/ca_buffer.h>
#include <munit/munit.h>
#include <bstring/bstrlib.h>
#include <errno.h>

char *lastfunc = NULL;

static void *buffer_tests_setup(const MunitParameter params[], void *user_data) {
  ca_buffer *buff = NULL;
  lastfunc = "ca_buffer_new";
  if(!ca_buffer_new(&buff)) goto error;
  if(buff==NULL) goto error;
  if(buff->mlen != (ca_buffer_def_mem)) goto error;
  if(buff->type != (ca_buffer_def_type)) goto error;

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
  lastfunc = "ca_buffer_new";
  if(!ca_buffer_new(&buff)) goto error;
  if(buff==NULL) goto error;
  if(buff->mlen != (ca_buffer_def_mem)) goto error;
  if(buff->type != (ca_buffer_def_type)) goto error;

  lastfunc = "ca_buffer_destroy 0";
  ca_buffer_destroy(&buff);
  if(buff!=NULL) goto error;

  lastfunc = "ca_buffer_new_m";
  if(!ca_buffer_new_m(&buff,128)) goto error;
  if(buff==NULL) goto error;
  if(buff->mlen!=128) goto error;
  if(buff->type != (ca_buffer_def_type)) goto error;

  lastfunc = "ca_buffer_destroy 1";
  if(!ca_buffer_destroy(&buff)) goto error;
  if(buff!=NULL) goto error;

  lastfunc = "ca_buffer_new_t";
  if(!ca_buffer_new_t(&buff,2,sizeof(ca_buffer))) goto error;
  if(buff->mlen!=(2*sizeof(ca_buffer))) goto error;
  if(buff->type != sizeof(ca_buffer)) goto error;

  lastfunc = "ca_buffer_destroy 2";
  ca_buffer_destroy(&buff);
  if(buff!=NULL) goto error;

  return MUNIT_OK;
error:
  printf("%s (%s) %s [error] %d:%s",__FILE__,__FUNCTION__,
         lastfunc,errno,strerror(errno));
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

  lastfunc = "ca_buffer_write";
  if(!ca_buffer_write(buff,input->data,input->mlen)) goto error;
  lastfunc = "memcmp";
  if(memcmp(input->data,buff->buff,input->mlen)!=0) goto error;

  memset(buff->buff,0,buff->mlen);

  lastfunc = "ca_buffer_write_t";
  if(!ca_buffer_write_t(buff,input->data,input->mlen,2)) goto error;
  lastfunc = "memcmp";
  if(memcmp(input->data,buff->buff,(2*buff->type))!=0) goto error;

  memset(buff->buff,0,buff->mlen);

  lastfunc = "ca_buffer_write_s";
  if(!ca_buffer_write_s(buff,10,input->data,input->mlen)) goto error;
  lastfunc = "calloc";
  void *zeroes = calloc(10,buff->type);
  if(zeroes==NULL) goto error;
  lastfunc = "memcmp zeroes";
  if(memcmp(zeroes,buff->buff,(10*buff->type))!=0) goto error;
  lastfunc = "memcmp input";
  if(memcmp(input->data,buff->buff+(10*buff->type),input->mlen)!=0) goto error;
  free(zeroes);

  buff->len=buff->mlen;
  lastfunc = "ca_buffer_full";
  if(!ca_buffer_full(buff)) goto error;
  lastfunc = "ca_buffer_data";
  if(ca_buffer_data(buff)!=buff->mlen) goto error;
  lastfunc = "ca_buffer_space";
  if(ca_buffer_space(buff)!=0) goto error;

  int len = buff->len;
  int mlen = buff->mlen;
  lastfunc = "ca_buffer_write_ex";
  if(!ca_buffer_write_ex(buff,input2->data,input2->mlen)) goto error;
  lastfunc = "chk len";
  if(buff->len!=input2->mlen) goto error;
  lastfunc = "chk mlen";
  if(buff->mlen<=mlen) goto error;
  lastfunc = "chk inplen";
  if(buff->mlen!=input2->mlen) goto error;
  lastfunc = "memcmp";
  if(memcmp(input2->data,buff->buff,input2->mlen)!=0) goto error;

  lastfunc = "ca_buffer_resize";
  if(!ca_buffer_resize(buff,mlen)) goto error;
  if(buff->mlen!=mlen) goto error;

  lastfunc = "ca_buffer_write_sex";
  if(!ca_buffer_write_sex(buff,mlen,input2->data,input2->mlen)) goto error;
  lastfunc = "chk len";
  if(buff->len!=(mlen+input2->mlen)) goto error;
  lastfunc = "chk mlen";
  if(buff->mlen<=mlen) goto error;
  lastfunc = "memcmp";
  if(memcmp(buff->buff+mlen,input2->data,input2->mlen)!=0) goto error;

  memset(buff->buff,0,buff->mlen);

  lastfunc = "ca_buffer_resize";
  if(!ca_buffer_resize(buff,mlen)) goto error;
  if(buff->mlen!=mlen) goto error;

  lastfunc = "ca_buffer_write_ext";
  if(!ca_buffer_write_ext(buff,input2->data,input2->mlen,input2->mlen/buff->type)) goto error;
  if(memcmp(buff->buff,input2->data,input2->mlen)!=0) goto error;

  memset(buff->buff,0,buff->mlen);

  lastfunc = "ca_buffer_write_chunk";
  if(!ca_buffer_write_ext(buff,input2->data,input2->mlen,input2->mlen/buff->type)) goto error;
  if(memcmp(buff->buff,input2->data,input2->mlen)!=0) goto error;
  

  bdestroy(input);
  bdestroy(input2);
  return MUNIT_OK;
error:
  if(input!=NULL) bdestroy(input);
  if(input2!=NULL) bdestroy(input2);
  printf("%s (%s) %s [error] %d:%s",__FILE__,__FUNCTION__,
         lastfunc,errno,strerror(errno));
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

  bdestroy(input);
  bdestroy(input2);
  return MUNIT_OK;
error:
  if(input!=NULL) bdestroy(input);
  if(input2!=NULL) bdestroy(input2);
  printf("%s (%s) %s [error] %d:%s",__FILE__,__FUNCTION__,
         lastfunc,errno,strerror(errno));
  return MUNIT_FAIL;
}

int main(int argc, char *const *argv) {
  MunitTest buffer_tests[] = {{"/new", buffer_test_new, buffer_tests_setup, buffer_test_teardown,
                        MUNIT_TEST_OPTION_NONE, NULL},
                       {"/write", buffer_test_write, buffer_tests_setup, buffer_test_teardown,
                        MUNIT_TEST_OPTION_NONE, NULL},
                       {"/read", buffer_test_read, buffer_tests_setup, buffer_test_teardown,
                        MUNIT_TEST_OPTION_NONE, NULL},
                       {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

  const MunitSuite buffer_suite = {"/buffer_tests", buffer_tests, NULL, 1,
                            MUNIT_SUITE_OPTION_NONE};

  return munit_suite_main(&buffer_suite, NULL, 0, NULL);
}
