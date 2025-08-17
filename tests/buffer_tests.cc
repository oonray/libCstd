#include <errno.h>
#include <munit/munit.h>
#include <bstring/bstrlib.h>
#include <error/ca_error.h>
#include <buffer/ca_buffer.hh>

static void *buffer_tests_setup(const MunitParameter params[], void *user_data) {
  ca::buffer *buff = new ca::buffer();

  ca_err_chk_c(buff->left() != ca::buffer_default::SIZE,ENOBUFS);
  ca_err_chk_c(buff->expandable(),ENOBUFS);
  ca_err_return_pointer(buff);
}

static void *buffer_tests_setup_expand(const MunitParameter params[], void *user_data) {
  ca::buffer *buff = new ca::buffer(ca::buffer_option::EXPAND);

  ca_err_chk_c(buff->left() != ca::buffer_default::SIZE,ENOBUFS);
  ca_err_chk_c(!buff->expandable(),ENOBUFS);
  ca_err_return_pointer(buff);
}

static void buffer_test_teardown(void *fixture) {
    if(fixture==NULL) return;
    delete (ca::buffer *)fixture;
}

MunitResult buffer_test_new(const MunitParameter params[],
                     void *user_data_or_fixture) {
  ca::buffer *buff = new ca::buffer(96*sizeof(char),ca::buffer_option::NOEXPAND);

  if(buff==NULL) goto error;
  if(buff->left() != (96*sizeof(char))) ca_err_throw();
  if(buff->expandable()) ca_err_throw();

  delete buff;
  ca_err_chk_not_null_c(buff,EPROTO);

  buff = new ca::buffer();
  if(buff==NULL) goto error;
  if(buff->left() != ca::buffer_default::SIZE) ca_err_throw();
  if(buff->expandable()) ca_err_throw();

  delete buff;
  ca_err_chk_not_null_c(buff,EPROTO);

  buff = new ca::buffer(ca::buffer_option::EXPAND);
  if(buff->left() != ca::buffer_default::SIZE) ca_err_throw();
  if(!buff->expandable()) ca_err_throw();

  delete buff;
  ca_err_chk_not_null_c(buff,EPROTO);

  ca_err_return_test();
}

MunitResult buffer_test_write(const MunitParameter params[],
                       void *user_data_or_fixture) {
}

MunitResult buffer_test_write_expand(const MunitParameter params[],
                       void *user_data_or_fixture) {
}

MunitResult buffer_test_read(const MunitParameter params[],
                      void *user_data_or_fixture) {
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
