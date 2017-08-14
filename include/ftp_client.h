/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/13 18:26:58 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/14 15:28:41 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_CLIENT_H
# define FTP_CLIENT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>

#include "ftp_network.h"
#include "ftp_errors.h"

void    ftp_init_client(int *sock, char *addr, char *port);
int     ftp_connect(struct addrinfo *p);

/* Server Interaction */
int		ftp_handle_user_commands(int sock);
int		ftp_run_command(int sock, char *buf);
char    *ftp_build_command(char *buf, char **cmd);
int     ftp_ls(int sock, char *cmd);
int     ftp_cd(int sock, char *cmd);
int     ftp_get(int sock, char *cmd);
int     ftp_put(int sock, char *cmd);
int     ftp_pwd(int sock, char *cmd);
int     ftp_quit(int sock);

#endif /* FTP_CLIENT_H */