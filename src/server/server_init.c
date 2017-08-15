/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 12:31:41 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/14 21:58:45 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include <sys/mman.h>
#include <unistd.h>

int     ftp_bind_socket(int sock, struct sockaddr *addr, socklen_t addrlen);

/* Initialises server - Basically creates listening socket */
void	init_server(t_server *server, int ac, char **av)
{
    struct sigaction	sa;
	char				*tmp;

	tmp = getcwd(NULL, 0);
	server->i.root_path = ft_strjoin(tmp, "/");
	ft_memdel((void **)&tmp);
	server->i.pwd = ft_strdup("/");
	// Initialises array
	int i = -1;
	while (++i < MAX_CLIENTS)
		server->client_array[i] = NULL;
	server->i.n_clients = 0;

	// Setup socket
	server->i.port = (ac < 2) ? PORT : av[1];
	ftp_get_listening_socket(&(server->listenSocket), server->i.port);
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
void	ftp_get_listening_socket(int *sock, char *port)
{
    struct addrinfo hints;
    struct addrinfo *p;
    struct addrinfo *servinfo;

    ft_memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if (getaddrinfo(NULL, port, &hints, &servinfo) != 0)
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