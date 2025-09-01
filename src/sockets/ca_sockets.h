#ifndef __ca_soc_h
#define __ca_soc_h

#include <bstring/bstrlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <buffer/ca_buffer.h>
#include <error/ca_error.h>

#define CA_SOCKET_USE_DEFAULT_PORT 0
#define CA_SOCKET_DEFAULT_PORT 42069

#define CA_SOCKET_USE_DEFAULT_IP ""
#define CA_SOCKET_DEFAULT_IP bfromcstr("0.0.0.0")

#define CA_SOCKET_DEFAULT_TYPE CA_SOCKET_IPV4
#define CA_SOCKET_USE_DEFAULT_TYPE 0
typedef enum _ca_socket_type {
    CA_SOCKET_UNIX=AF_UNIX,
    CA_SOCKET_IPV4=AF_INET,
    CA_SOCKET_IPV6=AF_INET6,
}ca_socket_type;

#define CA_SOCKET_DEFAULT_PROTOCOL CA_SOCKET_TCP
#define CA_SOCKET_USE_DEFAULT_PROTO 0
typedef enum _ca_socket_protocol {
    CA_SOCKET_TCP=SOCK_STREAM,
    CA_SOCKET_UDP=SOCK_DGRAM,
    CA_SOCKET_RAW=SOCK_RAW,
}ca_socket_protocol;

typedef enum _ca_socket_use {
    CA_SOCKET_SERVER,
    CA_SOCKET_CLIENT,
}ca_socket_use;

typedef struct sockaddr_in ca_socket_address;

#define CA_SOCKET_USE_DEFAULT_INFO NULL
typedef struct _ca_socket_info {
    bstring ip;
    int port;
    ca_socket_protocol proto;
    ca_socket_type type;
    ca_socket_use use;
}ca_socket_info;

typedef struct _ca_socket{
    int descriptor;
    ca_socket_info *info;
    ca_socket_address *addr;
}ca_socket;

typedef struct _ca_socket_client{
}ca_socket_client;

typedef struct _ca_socket_server{
    ca_socket *socket;
}ca_socket_server;

int ca_socket_info_new(
    ca_socket_info **out,
    bstring ip,
    int port,
    ca_socket_use use,
    ca_socket_protocol proto,
    ca_socket_type type);

int ca_socket_server_new(ca_socket_server **out,
                         ca_socket *socket);

int ca_socket_server_new(ca_socket_server **out,
                         ca_socket *soc);

#endif
