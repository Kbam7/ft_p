/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 12:31:41 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/18 10:24:14 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include <sys/mman.h>
#include <unistd.h>

void	ftp_get_listening_socket(int *sock, char *port);
int		ftp_create_socket(struct addrinfo *p);
void	init_func(t_server *server);

/* Initialises server - Basically creates listening socket */
void	init_server(t_server *server, int ac, char **av)
{
    struct sigaction	sa;

	init_func(server);

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

void	init_func(t_server *server)
{
	char	*tmp;
	int		i;

	i = -1;
	tmp = getcwd(NULL, 0);
	server->i.root_path = ft_strjoin(tmp, "/");
	ft_memdel((void **)&tmp);
	server->i.pwd = ft_strdup("/");
	while (++i < MAX_CLIENTS)
		server->client_array[i] = NULL;
	server->i.n_clients = 0;
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
	//struct timeval	tv;

	//tv.tv_sec = 1;
	//tv.tv_usec = 0;
    yes = 1;
    sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sock == -1)
        ftp_error(ERR_WARN, "socket: Unable to create socket");
    else {
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		//setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
		//setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));
        if (bind(sock, p->ai_addr, p->ai_addrlen) != -1)
			return (sock);
		close(sock);            
        ftp_error(ERR_WARN, "bind: Unable to bind");
	}
    return (-1);
}