/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 12:31:41 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/12 14:56:16 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

/* Initialises server */
void	init_server(t_server *server)
{
    struct sigaction    sa;
	int					i;
    
	i = -1;
	server->n_clients = 0;
	while (++i < MAX_CLIENTS)
		ft_memset(server->client_array[i], 0, sizeof(server->client_array[i]));
    ftp_get_listening_socket(server->listenSocket);
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
void	ftp_get_listening_socket(int sock)
{
    struct addrinfo hints;
    struct addrinfo *p;
    struct addrinfo *servinfo;

    ft_memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if (getaddrinfo(NULL, PORT, &hints, &servinfo) != 0)
        ftp_error(ERR_FATAL, "getaddrinfo");
    p = servinfo;
    while (p != NULL) {
        sock = ftp_create_socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock == -1)
            continue;
        if (ftp_bind_socket(sock, p->ai_addr, p->ai_addrlen) == -1)
            continue;
        break;
    }
    freeaddrinfo(servinfo);
    if (p == NULL)
        ftp_error(ERR_FATAL, "server: failed to bind");
}

/* Creates and configures a new socket */
int		ftp_create_socket(int fam, int type, int proto)
{
    int sock;
    int yes;

    yes = 1;
    if ((sock = socket(fam, type, proto)) == -1) {
        ftp_error(ERR_WARN, "server: unable to create socket");
        return (-1);
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
        ftp_error(ERR_FATAL, "sever: setsockopt: Unable to configure socket options");
    return (sock);
}

/* Binds a socket to specified address */
int ftp_bind_socket(int sock, const struct sockaddr *addr, socklen_t addrlen)
{
    if (bind(sock, addr, addrlen) == -1) {
        close(sock);            
        ftp_error(ERR_WARN, "server: unable to bind");
        return (-1);
    }
    return (sock);
}