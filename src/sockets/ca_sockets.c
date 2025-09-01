#include "ca_error.h"
#include <sockets/ca_sockets.h>
#include <sys/socket.h>

int ca_socket_info_new(
    ca_socket_info **out,
    bstring ip,
    int port,
    ca_socket_use use,
    ca_socket_protocol proto,
    ca_socket_type type){
    ca_err_chk_par_not_null(out);
    ca_err_chk_par_null(*out);

    *out = calloc(1,sizeof(ca_socket_address));
    ca_err_chk(out);

    (*out)->use = use;
    (*out)->type = type==CA_SOCKET_USE_DEFAULT_TYPE?
                        CA_SOCKET_DEFAULT_TYPE:type;
    (*out)->proto= proto == CA_SOCKET_USE_DEFAULT_PROTO?
                            CA_SOCKET_DEFAULT_PROTOCOL:proto;

    ca_err_return_bool();
}

int ca_socket_info_new_default(
    ca_socket_info **out,
    bstring ip,
    int port,
    ca_socket_use use){
    return ca_socket_info_new(out,ip,port,use,
                              CA_SOCKET_DEFAULT_PROTOCOL,
                              CA_SOCKET_DEFAULT_TYPE);
}

int ca_socket_new(ca_socket **out,
                  ca_socket_info *info){
    ca_err_chk_par_not_null(out);
    ca_err_chk_par_null(*out);

    *out = calloc(1,sizeof(ca_socket));
    ca_err_chk_null(*out);

    if(info == CA_SOCKET_USE_DEFAULT_INFO){
        if(!ca_socket_info_new_default(&(*out)->info,
                               CA_SOCKET_DEFAULT_IP,
                               CA_SOCKET_DEFAULT_PORT,
                               CA_SOCKET_SERVER)) ca_err_throw();
    }else{(*out)->info=info;}

    ca_err_return_bool();
}

int ca_socket_server_new(ca_socket_server **out,
                         ca_socket *socket){
    ca_err_chk_par_not_null(out);
    ca_err_chk_par_null((*out));

    *out = calloc(1,sizeof(ca_socket_server));
    ca_err_chk(out);


    (*out)->addr->sin_family =

    (*out)->addr->sin_port = htons((*out)->host->port);
    (*out)->addr->sin_addr.s_addr = inet_addr(bdata((*out)->host->ip));

    ca_err_return_bool();
}
