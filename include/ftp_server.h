/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 10:47:37 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/12 19:53:23 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_SERVER_H
# define FTP_SERVER_H

#include "libft.h"
#include "ftp_errors.h"
#include "ftp_signals.h"
#include "ftp_network.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

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
void    	ftp_get_listening_socket(int *sock);
int     	ftp_create_socket(struct addrinfo *p);
t_client	*ftp_accept_client(t_server *server);
void		ftp_disconnect_client(t_server *server, unsigned int i_client);
void    	ftp_handle_client(t_server *server, t_client *client);

#endif /* FTP_SERVER_H */