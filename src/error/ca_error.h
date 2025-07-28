#ifndef _ca_error_h
#define _ca_error_h 1

#include <bstring/bstrlib.h>
#include <errno.h>
#include <error/ca_colors.h>

#ifdef CUSTOM_CA_ERRORS
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
  printf("(%s:%s%d%s) [error %s%d%s] %s\n",\
         ca_err.function,\
         CA_ANSI_COLOR_RED,\
         ca_err.line,\
         CA_ANSI_COLOR_REMOVE,\
         CA_ANSI_COLOR_RED,\
         errno,\
         CA_ANSI_COLOR_REMOVE,\
         strerror(errno))

#define ca_err_print_test()\
  printf("\r(%s:%s%d%s) [error %s%d%s] %s\n",\
         ca_err.function,\
         CA_ANSI_COLOR_RED,\
         ca_err.line,\
         CA_ANSI_COLOR_REMOVE,\
         CA_ANSI_COLOR_RED,\
         errno,\
         CA_ANSI_COLOR_REMOVE,\
         strerror(errno))

#else
#define ca_err_reset() errno=0;
#define ca_err_set()

#define ca_err_throw() goto error;
#define ca_err_throw_c(err) {errno=err;goto error;}
#define ca_err_throw_cl(err,LABEL) {errno=err;goto LABEL;}

#define ca_err_print()\
  printf("[error %s%d%s] %s\n",\
         CA_ANSI_COLOR_RED,\
            errno,strerror(errno),\
         CA_ANSI_COLOR_REMOVE)

#define ca_err_print_test()\
  printf("\r[error %s%d%s] %s\n",\
         CA_ANSI_COLOR_RED,\
            errno,strerror(errno),\
         CA_ANSI_COLOR_REMOVE)
#endif
#endif
