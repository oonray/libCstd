#include <stdio.h>
#include <errno.h>


#define ca_chk_null(VAR, ERRNO){\
    if(VAR==NULL){\
        errno=ERRNO;\
        return 0;\
    }\
}
#define ca_chk_not_null(VAR, ERRNO){\
    if(VAR!=NULL){\
        errno=ERRNO;\
        return 0;\
    }\
}

#define ca_chk_arg_not_null(ARG) ca_chk_not_null(ARG,EINVAL)
#define ca_chk_arg_null(ARG) ca_chk_not_null(ARG,EINVAL)
