/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 10:27:25 by kbamping          #+#    #+#             */
/*   Updated: 2017/08/15 13:23:03 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include <sys/types.h>
#include <dirent.h>

/*
ftp_error(ERR_INFO, s->i.root_path);
ftp_error(ERR_INFO, s->i.pwd);
ftp_error(ERR_INFO, args);
*/

int     ftp_cd(t_server *s, int sock, char *args)
{
	char	*new_cwd;
	char	*tmp;
	int		rv;

	rv = -1;
	args = (args == NULL) ? s->i.root_path : args;
	tmp = ftp_get_path(s, args);
	if (ftp_validate_path(s->i.root_path, tmp)) {
		if (chdir(tmp) == 0) {
			ftp_get_cwd(&new_cwd);
			tmp = new_cwd + ft_strlen(s->i.root_path);
			rv = ftp_send_data(sock, tmp, 26);
			ft_memdel((void **)&s->i.pwd);
			s->i.pwd = tmp;
			ft_memdel((void **)&new_cwd);
		} else 
			rv = ftp_send_data(sock, "Unable to change directory", 26);
	}
    return (rv);
}

/* int		ftp_cd_validate_cwd(t_server *s, int sock, char *cwd, char *new_cwd)
{
	char	*tmp;
	int		rp_len;
	int		rv;
	
	rp_len = ft_strlen(s->i.root_path);
	if (ft_strncmp(new_cwd, s->i.root_path, rp_len) == 0) {
		// new cwd contains atleast the root dir
		if ((rv = ftp_send_data(sock, "Success! Directory changed", 26)) < 1)
			return (rv);
		// get ptr to begining of path, relative to root_path
		tmp = new_cwd + rp_len;
		ft_memdel((void **)&s->i.pwd);
		s->i.pwd = ft_strdup(((*tmp == '\0') ? "/" : tmp));
	} else if (chdir(cwd) == 0) {
		if ((rv = ftp_send_data(sock, "Unable to change directory", 26)) < 1)
			return (rv);
	} else {
		ftp_send_data(sock, UNAUTHORISED_AREA, ft_strlen(UNAUTHORISED_AREA));
		ftp_error(ERR_FATAL, "cd: unauthorised path accessed, killing client");
	}
	return (1);
} */