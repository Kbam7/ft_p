/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 10:27:25 by kbamping          #+#    #+#             */
/*   Updated: 2017/08/20 12:58:02 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include <sys/types.h>
#include <dirent.h>

int		ftp_handle_new_dir(t_server *s, int sock)
{
	char	*tmp;
	char	*new_cwd;

	ftp_get_cwd(&new_cwd);
	if ((tmp = new_cwd + ft_strlen(s->i.root_path)) && *tmp == 0)
		tmp = "/";
	ft_memdel((void **)&(s->i.pwd));
	s->i.pwd = (tmp[0] == '/') ? ft_strdup(tmp) : ft_strjoin("/", tmp);
	ft_memdel((void **)&new_cwd);
	return (ftp_send_data(sock, s->i.pwd, ft_strlen(s->i.pwd)));
}

int		ftp_cd(t_server *s, int sock, char *args)
{
	char	*tmp;
	int		rv;

	rv = -1;
	args = (args == NULL) ? "/" : args;
	tmp = ftp_get_path(s, args);
	if (ftp_validate_path(s->i.root_path, tmp))
	{
		if (chdir(tmp) == 0)
			rv = ftp_handle_new_dir(s, sock);
		else
			rv = ftp_send_data(sock, "Unable to change directory", 26);
	}
	else
		rv = ftp_send_data(sock, "Invalid path", 12);
	ft_memdel((void **)&tmp);
	return (rv);
}
