#include "ftp_network.h"

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int ftp_send_msg(int sock, char *buf, int len)
{
    int     rv;

    if ((rv = send(sock, buf, len, 0)) == -1)
        ftp_error(ERR_WARN, "send: Failed to send data");
    if  (rv == 0)
        ftp_error(ERR_WARN, "send: zero bytes sent"); // ?
    return (rv);
}

int ftp_receive_msg(int sock, char (*buf)[])
{
    int     rv;
    
    if ((rv = recv(sock, *buf, MAX_MSGSIZE, 0)) == -1)
        ftp_error(ERR_WARN, "read: Failed to read data");
    if  (rv == 0)
        ftp_error(ERR_WARN, "read: zero bytes read"); //  Client disconnected?
    return (rv);
}