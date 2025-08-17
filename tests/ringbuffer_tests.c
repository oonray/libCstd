#include "ca_error.h"
#include <ca_ringbuffer.h>
#include <munit/munit.h>

static void *tests_setup(const MunitParameter params[], void *user_data) {
  ca_ringbuffer *buff = NULL;
  ca_err_chk_res(!ca_ringbuffer_new_default(&buff));
  ca_err_chk_par_null(buff);

  ca_err_return_pointer(buff);
}

static void test_teardown(void *fixture) { 
    if(fixture==NULL) return;
    ca_ringbuffer_destroy((ca_ringbuffer **)&fixture); }

MunitResult test_new(const MunitParameter params[],
                     void *user_data_or_fixture) {
  ca_ringbuffer *buff = NULL;
  ca_err_chk_res(!ca_ringbuffer_new_default(&buff));
  ca_err_chk_par_null_c(buff,EPROTO);
  ca_err_chk_null_c(buff->buffer,EPROTO);

  ca_err_chk_res(!ca_ringbuffer_destroy(&buff));
  ca_err_chk_not_null_c(buff,EPROTO);

  ca_err_return_test();
}

MunitResult test_write(const MunitParameter params[],
                       void *user_data_or_fixture) {
    ca_ringbuffer *b = (ca_ringbuffer *)user_data_or_fixture;
    ca_err_chk_null_c(b,EPROTO);
    ca_err_chk_null_c(b->buffer,EPROTO);

    bstring data = bfromcstr("hello world!");
    ca_err_chk_res(!ca_ringbuffer_write(b,data->data,data->mlen));
    ca_err_chk_c(b->end!=data->mlen, EPROTO);
    ca_err_chk_c(b->start>0, EPROTO);
    ca_err_chk_c(
        memcmp(ca_ringbuffer_starts_at(b),data->data,data->mlen) != 0,
        EPROTO);

    ca_err_chk_res(!ca_ringbuffer_write(b,data->data,data->mlen));
    ca_err_chk_c(b->end!=(data->mlen*2), EPROTO);
    ca_err_chk_c(b->start>0, EPROTO);
    ca_err_chk_c(
        memcmp(ca_ringbuffer_starts_at(b),data->data,data->mlen) != 0,
        EPROTO);
    ca_err_chk_c(
        memcmp(ca_ringbuffer_starts_at(b)+data->mlen,data->data,data->mlen) != 0,
        EPROTO);

    bdestroy(data);
    return MUNIT_OK;
error:
    ca_err_print_test();
    bdestroy(data);
    return MUNIT_FAIL;
}

MunitResult test_read(const MunitParameter params[],
                      void *user_data_or_fixture) {
    ca_ringbuffer *b = (ca_ringbuffer *)user_data_or_fixture;
    ca_err_chk_null_c(b,EPROTO);
    ca_err_chk_null_c(b->buffer,EPROTO);

    bstring data = bfromcstr("hello world!");
    bstring data2 = bfromcstr("hello world again!");

    bstring out = calloc(1,sizeof(struct tagbstring));
    ca_err_chk_null(out);
    out->mlen=data2->mlen;
    out->slen=data2->slen;
    out->data=calloc(1,sizeof(out->mlen));
    ca_err_chk_null(out->data);

    size_t outsize=0;
    unsigned char *outalloc = NULL;

    ca_err_chk_res(!ca_ringbuffer_write(b,data->data,data->mlen));
    ca_err_chk_res(!ca_ringbuffer_write(b,data2->data,data2->mlen));

    ca_err_chk_res(!ca_ringbuffer_read(b,out->data,data->mlen));
    ca_err_chk_c(
        memcmp(out->data,data->data,data->mlen) != 0,
        EPROTO);

    memset(out->data,0,out->mlen);

    ca_err_chk_res(!ca_ringbuffer_read(b,out->data,data2->mlen));
    ca_err_chk_c(
        memcmp(out->data,data2->data,data2->mlen) != 0,
        EPROTO);

    ca_err_chk_res(!ca_ringbuffer_write(b,data->data,data->mlen));
    ca_err_chk_res(!ca_ringbuffer_write(b,data2->data,data2->mlen));

    fprintf(stderr,"\n\rfree\n");
    free(outalloc);
    outalloc=NULL;


    bdestroy(data);
    return MUNIT_OK;
error:
    ca_err_print_test();
    bdestroy(data);
    return MUNIT_FAIL;
}

int main(int argc, char *const *argv) {
   MunitTest tests[] = {{"/test_new", test_new, NULL, NULL,
                        MUNIT_TEST_OPTION_NONE, NULL},
                       {"/test_write", test_write, tests_setup, test_teardown,
                        MUNIT_TEST_OPTION_NONE, NULL},
                       {"/test_read", test_read, tests_setup, test_teardown,
                        MUNIT_TEST_OPTION_NONE, NULL},
                       {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

  const MunitSuite suite = {"/Ringbuffer Tests", tests, NULL, 1,
                            MUNIT_SUITE_OPTION_NONE};

  return munit_suite_main(&suite, NULL, 0, NULL);
}
