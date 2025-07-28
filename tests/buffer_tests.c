#include <munit/munit.h>
#include <bstring/bstrlib.h>
#include <error/ca_error.h>
#include <buffer/ca_buffer.h>

static void *buffer_tests_setup(const MunitParameter params[], void *user_data) {
  ca_buffer *buff = NULL;
  if(!ca_buffer_new_default(&buff)) goto error;
  if(buff==NULL) goto error;
  if(buff->mlen != CA_BUFFER_DEF_SIZE) ca_err_throw();
  if(buff->opt != CA_BUFFER_NOEXPAND) ca_err_throw();

  return buff;
error:
  return NULL;
}

static void *buffer_tests_setup_expand(const MunitParameter params[], void *user_data) {
  ca_buffer *buff = NULL;
  if(!ca_buffer_new_default_expand(&buff)) goto error;
  if(buff==NULL) goto error;
  if(buff->mlen != CA_BUFFER_DEF_SIZE) ca_err_throw();
  if(buff->opt != CA_BUFFER_EXPAND) ca_err_throw();

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
  if(!ca_buffer_new(&buff,96*sizeof(char),CA_BUFFER_NOEXPAND))
        goto error;

  if(buff==NULL) goto error;
  if(buff->mlen != (96*sizeof(char))) ca_err_throw();
  if(buff->opt != CA_BUFFER_NOEXPAND) ca_err_throw();

  ca_buffer_destroy(&buff);
  if(buff!=NULL) goto error;

  if(!ca_buffer_new_default(&buff)) goto error;
  if(buff==NULL) goto error;
  if(buff->mlen!=CA_BUFFER_DEF_SIZE)  ca_err_throw();
  if(buff->opt != CA_BUFFER_NOEXPAND) ca_err_throw();

  if(!ca_buffer_destroy(&buff)) goto error;
  if(buff!=NULL) goto error;

  if(!ca_buffer_new_default_expand(&buff)) goto error;
  if(buff->mlen!=CA_BUFFER_DEF_SIZE)  ca_err_throw();
  if(buff->opt != CA_BUFFER_EXPAND)   ca_err_throw();

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
  input2->mlen=input->mlen;
  input2->slen=input2->mlen/sizeof(char);
  input2->data=calloc(1,input2->mlen);
  if(input2->data==NULL) goto error;
  memset(input2->data,'A',input->mlen);

  if(!ca_buffer_write(buff,input->data,input->mlen)) ca_err_throw();
  if(memcmp(buff->data,input->data,input->mlen) != 0)
        ca_err_throw_c(EINVAL);

  if(!ca_buffer_write(buff,input2->data,input2->mlen)) ca_err_throw();
  if(!ca_buffer_write_end(buff,input->data,input->mlen)) ca_err_throw();
  if(memcmp(buff->data+input2->mlen,input->data,input->mlen) != 0)
        ca_err_throw_c(EINVAL);

  bdestroy(input);
  bdestroy(input2);
  return MUNIT_OK;
error:
  if(input!=NULL) bdestroy(input);
  if(input2!=NULL) bdestroy(input2);
  ca_err_print_test();
  return MUNIT_FAIL;
}

MunitResult buffer_test_write_expand(const MunitParameter params[],
                       void *user_data_or_fixture) {
  ca_buffer *buff = (ca_buffer *)user_data_or_fixture;
  if(buff==NULL) return MUNIT_FAIL;

  bstring input = bfromcstr("Hello World");
  bstring input2 = calloc(1,sizeof(struct tagbstring));
  if(input2==NULL) goto error;
  input2->mlen=buff->mlen;
  input2->slen=input2->mlen/sizeof(char);
  input2->data=calloc(1,input2->mlen);
  if(input2->data==NULL) goto error;
  memset(input2->data,'A',input->mlen);

  int orig_len = buff->mlen;

  if(!ca_buffer_write(buff,input2->data,input2->mlen)) ca_err_throw();
  if(!ca_buffer_full(buff)) ca_err_throw_c(EINVAL);

  if(!ca_buffer_write_end(buff,input->data,input->mlen)) ca_err_throw();
  if(memcmp(buff->data+input2->mlen,input->data,input->mlen) != 0)
        ca_err_throw_c(EINVAL);

  if(buff->mlen != orig_len) ca_err_throw_c(EINVAL);

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
  MunitTest buffer_tests[] = {
{"/new",buffer_test_new, 
            buffer_tests_setup, buffer_test_teardown,
            MUNIT_TEST_OPTION_NONE, NULL},

{"/write",buffer_test_write,
            buffer_tests_setup, buffer_test_teardown,
            MUNIT_TEST_OPTION_NONE, NULL},

{"/write_expand",buffer_test_write_expand,
            buffer_tests_setup_expand, buffer_test_teardown,
            MUNIT_TEST_OPTION_NONE, NULL},

{"/read\n", buffer_test_read,
            buffer_tests_setup, buffer_test_teardown,
            MUNIT_TEST_OPTION_NONE, NULL},

{NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
    };

  const MunitSuite buffer_suite = {"/buffer_tests", buffer_tests, NULL, 1,
                            MUNIT_SUITE_OPTION_NONE};

  return munit_suite_main(&buffer_suite, NULL, 0, NULL);
}
