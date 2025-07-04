#ifndef _ca_error_h
#define _ca_error_h 1

#include <bstring/bstrlib.h>
#include <errno.h>

#ifdef __CA_CUSTOM_ERRORS
#undef errno
#define errno (*__ca_errno_location())
extern int ca_errno;
int *__ca_errno_location();
#endif

typedef struct _ca_error {
    char *function;
    int line;
} ca_error;

extern ca_error ca_err;

#ifdef CA_ERRORS
#define ca_err_reset() ca_err.function=NULL;ca_err.line=0;errno=0
#define ca_err_set() ca_err.function=(char *)&__func__;ca_err.line=__LINE__

#define ca_err_throw() {ca_err_set(); goto error;}
#define ca_err_throw_c(err) {errno=err;ca_err_set(); goto error;}
#define ca_err_throw_cl(err,LABEL) {errno=err;ca_err_set(err); goto LABEL;}

#define ca_err_print()\
  printf("(%s:%d) [error %d] %s\n",\
         ca_err.function,ca_err.line,\
         errno,strerror(errno));

#define ca_err_print_test()\
  printf("\r(%s:%d) [error %d] %s\n",\
         ca_err.function,ca_err.line,\
         errno,strerror(errno));

#else
#define ca_err_reset() errno=0;
#define ca_err_set()

#define ca_err_throw() goto error;
#define ca_err_throw_c(err) {errno=err;goto error;}
#define ca_err_throw_cl(err,LABEL) {errno=err;goto LABEL;}

#define ca_err_print()\
  printf("[error %d] %s\n",\
         errno,strerror(errno));

#define ca_err_print_test()\
  printf("\r[error %d] %s\n",\
         errno,strerror(errno));
#endif
#endif
