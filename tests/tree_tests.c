#include <munit.h>
#include <errno.h>
#include <strings.h>

#include <stdio.h>

#define CA_TREE_IMPL
#include <ca_tree.h>
#include <bstring/bstrlib.h>

MunitResult test_data_new(const MunitParameter params[],
                     void *user_data_or_fixture) {
  ca_data *v=NULL;
  bstring err;
  if(!ca_data_new(&v, CA_DATA_NONE, CA_DATA_NONE,CA_DATA_DEF_MLEN, CA_DATA_DEF_TYPE)){
        err=bfromcstr("Could not create data");
        goto error;}

  if(v==NULL){ 
        err=bfromcstr("ca_data is null");
        goto error;}

  if(v->bytes==NULL){
        err=bfromcstr("bytes is null");
        goto error;}

  if(v->len.items!=CA_DATA_DEF_ITEMS){
        err=bformat("len is wrong: %lu!=%lu",v->len.items,CA_DATA_DEF_ITEMS);
        goto error;}

  if(v->len.memory!=CA_DATA_DEF_MLEN){
        err=bformat("mlen is wrong: %lu!=%lu",v->len.memory,CA_DATA_DEF_MLEN);
        goto error;}

  if(!ca_data_del(v)) err=bfromcstr("Could not delete data");

  return MUNIT_OK;
error:
  if(err!=NULL) munit_log(MUNIT_LOG_ERROR,bdata(err));
  bdestroy(err);
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
