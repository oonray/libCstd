#include <munit.h>
#include <errno.h>
#include <strings.h>

#include <stdio.h>

#define CA_TREE_IMPL
#include <ca_tree.h>

#define fail(MSG) {fprintf(stderr,"\n" MSG "\n");goto fail;}
#define fail_err(MSG) {fprintf(stderr,"\n" MSG "%s \n", strerror(errno));goto fail;}

MunitResult test_data_new(const MunitParameter params[],
                     void *user_data_or_fixture) {
  ca_data *v=NULL;
  if(!ca_data_new(&v, CA_DATA_DEF_ILEN , CA_DATA_DEF_TYPE)) fail_err("Could not create data");

  if(v==NULL) fail("ca_data is null");
  if(v->bytes==NULL) fail("bytes is null")
  if(v->len!=CA_TREE_DEF_LEN) fail("len is wrong");
  if(v->mlen!=CA_TREE_DEF_LEN*CA_TREE_DEF_TYPE) fail("mlen is wrong");

  if(!ca_data_del(v)) fail_err("Could not delete data");

  return MUNIT_OK;
fail:
  return MUNIT_FAIL;
}

int main(int argc, char *argv[]) {
  MunitTest data_tests[] = {
      {"tree/test_data_new", test_data_new, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
      {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
    };

  MunitSuite data_suite[] = {
        {"data/", data_tests, NULL, 0, MUNIT_SUITE_OPTION_NONE},
        {NULL,NULL,NULL,0,MUNIT_SUITE_OPTION_NONE}
    };

  const MunitSuite all = {"CA Tests/", NULL, data_suite, 0, MUNIT_SUITE_OPTION_NONE};

  return munit_suite_main(&all, NULL, 0, NULL);
}
