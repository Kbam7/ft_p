/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/13 18:26:58 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/19 17:57:52 by kbamping         ###   ########.fr       */
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

typedef struct	s_ftp_session
{
	int				socket;
	struct in_addr	*address;
	char			*port;
	char			*root_path;
}				t_session;

void	ftp_init_client(t_session *s, int *sock, char *addr);

/* Server Interaction */
int		ftp_handle_user_commands(t_session *s);

/* Commands */
int		ftp_ls(int sock, char *cmd);
int		ftp_cd(int sock, char *cmd);
int		ftp_get(int sock, char *cmd);
int		ftp_put(int sock, char *cmd);
int		ftp_pwd(int sock, char *cmd);
int		ftp_quit(int sock);

/* Extra Commands */
int		ftp_mput(int sock, char *cmd);
int		ftp_mget(int sock, char *cmd);
int		ftp_lpwd(void);
int		ftp_lls(char *cmd);
int		ftp_lcd(char *cmd);

/* UI/Prompts */
int		ftp_print_prompt(void);
int		ftp_print_client_menu(void);

#endif /* FTP_CLIENT_H */