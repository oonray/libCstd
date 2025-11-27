#ifndef _ASOCK_H
#define _ASOCK_H

#include <ca_error.h>
#include <bstring/bstrlib.h>
#include <io/ca_io.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <mongoose/mongoose.h>

#define CA_DEF_ADDR INADDR_ANY

#define _CA_SOC_TYPE \
 _CA_IO_TYPE_X(CA_SOC,0xCA1,AF_INET) \
 _CA_IO_TYPE_X(CA_PIPE,0xCA2,AF_UNIX)

#define _CA_IO_TYPE_X(A,B,C)\
    A=B,

typedef enum _ca_soc_type {
    _CA_SOC_TYPE
} ca_soc_type;

#undef _CA_IO_TYPE_X

typedef enum _ca_soc_proto {
    CA_TCP=SOCK_STREAM,
    CA_UDP=SOCK_DGRAM,
} ca_soc_proto;

typedef struct _ca_listener {
    bstring host;
    int port;
    struct sockaddr_in addr;
}ca_listener;

typedef struct _ca_soc {
  ca_listener listener;
  int sock;
#ifdef HEADER_SSL_H
  SSL *ssl;
#endif
} ca_soc;

int ca_soc_new(ca_soc **out);
void ca_soc_destroy(ca_soc *soc);

int ca_lisener_set(ca_soc *soc,ca_soc_type type, ca_soc_proto proto, bstring host,int port);

int *ca_soc_accept(ca_soc *srv, int type);

int ca_soc_bind(ca_soc *srv);
int ca_soc_listen(ca_soc *srv, int backlog);
int ca_soc_connect(ca_soc *srv);

int ca_sock_read(ca_soc *soc);

#endif
