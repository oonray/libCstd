#include <munit/munit.h>
#include <bstring/bstrlib.h>
#include <stdio.h>
#include <errno.h>

#include <error/ca_error.h>

MunitResult error_test_new(const MunitParameter params[],
                     void *user_data_or_fixture) {

  errno=ENODATA;
  ca_err_set();
  if(errno!=ENODATA) ca_err_throw_c(EINVAL);
  ca_err_print_test();

  errno = ENODATA;
  if(errno!=ENODATA) ca_err_throw_c(EINVAL);
  ca_err_print_test();

  ca_err_reset();
  if(errno!=0) ca_err_throw_c(EINVAL);
  ca_err_print_test();

  return MUNIT_OK;
error:
  ca_err_print_test();
  return MUNIT_FAIL;
}

MunitResult error_test_alloc(const MunitParameter params[],
                     void *user_data_or_fixture) {

  void *errmem = calloc(-1,-5);
  if(errno!=0) ca_err_throw();

  return MUNIT_FAIL;
error:
  ca_err_print_test();
  return MUNIT_OK;
}

int main(int argc, char *const *argv) {
  MunitTest error_tests[] = {
                      {"/new\n\r", error_test_new,
                        NULL,
                        NULL,
                        MUNIT_TEST_OPTION_NONE, NULL},

                      {"/syserr\n\r", error_test_alloc,
                        NULL,
                        NULL,
                        MUNIT_TEST_OPTION_NONE, NULL},

                       {NULL, NULL, NULL, NULL,
                        MUNIT_TEST_OPTION_NONE, NULL}};

  const MunitSuite error_suite = {"/error_tests", error_tests, NULL, 1,
                            MUNIT_SUITE_OPTION_NONE};

  return munit_suite_main(&error_suite, NULL, 0, NULL);
}
