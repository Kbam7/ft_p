/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 10:27:25 by kbamping          #+#    #+#             */
/*   Updated: 2017/08/16 17:31:08 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include <sys/types.h>
#include <dirent.h>

int     ftp_cd(t_server *s, int sock, char *args)
{
	char	*new_cwd;
	char	*tmp;
	int		rv;

	rv = -1;
	args = (args == NULL) ? "/" : args;
	tmp = ftp_get_path(s, args);
	if (ftp_validate_path(s->i.root_path, tmp)) {
		if (chdir(tmp) == 0) {
			ft_memdel((void **)&tmp);
			ftp_get_cwd(&new_cwd);
			if ((tmp = new_cwd + ft_strlen(s->i.root_path)) && *tmp == 0)
				tmp = "/";
			ft_memdel((void **)&(s->i.pwd));
			s->i.pwd = (tmp[0] == '/') ? ft_strdup(tmp) : ft_strjoin("/", tmp);
			ft_memdel((void **)&new_cwd);
			rv = ftp_send_data(sock, s->i.pwd, ft_strlen(s->i.pwd));
		} else 
			rv = ftp_send_data(sock, "Unable to change directory", 26);
	} else {
		ft_memdel((void **)&tmp);
		rv = ftp_send_data(sock, "Invalid path", 12);
	}
    return (rv);
}