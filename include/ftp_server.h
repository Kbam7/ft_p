/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 10:47:37 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/12 14:59:29 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_SERVER_H
# define FTP_SERVER_H

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "libft.h"
#include "ftp_errors.h"
#include "ftp_signals.h"

#define PORT        	"54000"     // the port users will be connecting to
#define BACKLOG     	10          // how many pending connections queue will hold
#define MAX_CLIENTS 	1000        // Max number of clients allowed on server
#define MAX_READSIZE	1024

typedef	struct	s_ftp_client {
	unsigned int	index;
	int				socket;
	struct in_addr	*address;
    socklen_t		size;
	pid_t			pid;
	
}				t_client;

typedef	struct	s_ftp_server {
	
	int				listenSocket;
	unsigned int	n_clients;
	t_client		*client_array[MAX_CLIENTS];

}				t_server;

void 		init_server(t_server *server);
void    	ftp_get_listening_socket(int sock);
int     	ftp_create_socket(int fam, int type, int proto);
int     	ftp_bind_socket(int sock, const struct sockaddr *addr, socklen_t addrlen);
void		*get_in_addr(struct sockaddr *sa);
t_client	*ftp_accept_client(t_server *server);
void		ftp_disconnect_client(t_server *server, unsigned int i_client);
void    	ftp_handle_client(t_server *server, t_client *client);
void		ftp_handle_client_input(int sock);

#endif /* FTP_SERVER_H */