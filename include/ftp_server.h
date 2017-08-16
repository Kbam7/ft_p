/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 10:47:37 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/16 15:10:56 by kbamping         ###   ########.fr       */
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
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

typedef	struct	s_ftp_client {
	unsigned int	index;
	int				socket;
	struct in_addr	*address;
    socklen_t		size;
	pid_t			pid;
	
}				t_client;

typedef struct	s_server_information {
	char			*port;
	unsigned int	n_clients;
	char 			*root_path;
	char			*pwd;
}				t_serv_info;

typedef	struct	s_ftp_server {
	
	int			listenSocket;
	t_client	*client_array[MAX_CLIENTS];
	t_serv_info	i;

}				t_server;

/* Init */
void 		init_server(t_server *server, int ac, char **av);
t_client	*ftp_accept_client(t_server *server);
void		ftp_disconnect_client(t_server *server, unsigned int i_client);


/* Handle Client */
void    	ftp_handle_client(t_server *server, t_client *client);

/* Server Commands */
int     	ftp_ls(t_server *s, int sock, char *args);
int     	ftp_cd(t_server *s, int sock, char *args);
int     	ftp_get(t_server *s, int sock, char *args);
int     	ftp_put(t_server *s, int sock, char *args);
int     	ftp_pwd(t_server *s, int sock, char *args);
int     	ftp_quit(void);

/* Path Validation */
int			ftp_validate_path(char *root, char *path);

/* Command Utils */
void		ftp_get_cwd(char **cwd);
char		*ftp_get_path(t_server *s, char *args);

#endif /* FTP_SERVER_H */