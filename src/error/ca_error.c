#include <error/ca_error.h>

ca_error ca_err = {0};

#ifdef CUSTOM_CA_ERRORS
int ca_errno = 0;
int *__ca_errno_location(){
    int *errnop = __errno_location();
    if(*errnop!=0) ca_errno = *errnop;
    return &ca_errno;
}
#endif
