#include <munit.h>
#include <ca_vector.h>
#include <errno.h>
#include <strings.h>

MunitResult test_new(const MunitParameter params[],
                     void *user_data_or_fixture) {

  ca_vector *v=NULL;
  if(!ca_vector_new(&v, CA_VECTOR_DEF)){
        fprintf(stderr,"Could not create vector: %s", strerror(errno));
        return MUNIT_FAIL;
  }
  return MUNIT_OK;
}

int main(int argc, char *argv[]) {
  MunitTest tests[] = {
      {" test_new", test_new, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
      {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

  const MunitSuite suite = {"ca_vector Tests", tests, NULL, 1,
                            MUNIT_SUITE_OPTION_NONE};

  return munit_suite_main(&suite, NULL, 0, NULL);
}
