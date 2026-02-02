#include <munit.h>
#include <errno.h>
#include <strings.h>

#include <stdio.h>

#define CA_TREE_IMPL
#include <ca_tree.h>

MunitResult test_data_new(const MunitParameter params[],
                     void *user_data_or_fixture) {

  ca_data *v=NULL;
  if(!ca_data_new(&v, 1, sizeof(unsigned char))){
        fprintf(stderr,"Could not create vector: %s", strerror(errno));
        return MUNIT_FAIL;
  }

  if(!ca_data_del(v)){
        fprintf(stderr,"Could not delete vector: %s", strerror(errno));
        return MUNIT_FAIL;
  }

  return MUNIT_OK;
}

int main(int argc, char *argv[]) {
  MunitTest tests[] = {
      {"test_data_new", test_data_new, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
      {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

  const MunitSuite suite = {"CA Tests", tests, NULL, 1,
                            MUNIT_SUITE_OPTION_NONE};

  return munit_suite_main(&suite, NULL, 0, NULL);
}
