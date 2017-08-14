/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 12:31:41 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/13 17:19:12 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include <sys/mman.h>
#include <unistd.h>

int     ftp_bind_socket(int sock, struct sockaddr *addr, socklen_t addrlen);

/* Initialises server - Basically creates listening socket */
void	init_server(t_server *server)
{
    struct sigaction    sa;

    
    off_t offset, pa_offset;

    offset = 3;
    pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
printf("------[%li]---------\n", pa_offset);



	server->n_clients = 0;
    ftp_get_listening_socket(&(server->listenSocket));
    if (listen(server->listenSocket, BACKLOG) == -1)
        ftp_error(ERR_FATAL, "listen: Failed to listen to listen-socket");

    // Set handler for zombie processes
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        ftp_error(ERR_FATAL, "sigaction: Could not set handler for SIGCHLD");
}

/* Handles listening-socket creation. Listening-socket is used for new client connections */
void	ftp_get_listening_socket(int *sock)
{
    struct addrinfo hints;
    struct addrinfo *p;
    struct addrinfo *servinfo;

    ft_memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if (getaddrinfo(NULL, PORT, &hints, &servinfo) != 0)
        ftp_error(ERR_FATAL, "getaddrinfo: Failed to get server info");
    p = servinfo;
    while (p != NULL) {
        if ((*sock = ftp_create_socket(p)) == -1) {
            p = p->ai_next;
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo);
    if (p == NULL)
        ftp_error(ERR_FATAL, "failed to bind lstening-socket");
}

/* Creates and configures a new socket */
int		ftp_create_socket(struct addrinfo *p)
{
    int sock;
    int yes;

    yes = 1;
    sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sock == -1)
        ftp_error(ERR_WARN, "socket: Unable to create socket");
    else if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
        ftp_error(ERR_WARN, "setsockopt: Unable to configure socket options");
    else
        return (ftp_bind_socket(sock, p->ai_addr, p->ai_addrlen));
    return (-1);
}

/* Binds a socket to specified address */
int ftp_bind_socket(int sock, struct sockaddr *addr, socklen_t addrlen)
{
    if (bind(sock, addr, addrlen) == -1) {
        close(sock);            
        ftp_error(ERR_WARN, "bind: Unable to bind");
        return (-1);
    }
    return (sock);
}