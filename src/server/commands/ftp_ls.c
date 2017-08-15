/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_ls.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 10:25:41 by kbamping          #+#    #+#             */
/*   Updated: 2017/08/15 13:40:54 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include <sys/types.h>
#include <dirent.h>

int		ftp_ls_readdir(char **ret, char *tmp);

int     ftp_ls(t_server *s, int sock, char *args)
{
	char			*response;
	char			*tmp;
	int				rv;

	rv = 1;
	args = (args == NULL) ? s->i.pwd : args;
	tmp = ftp_get_path(s, args);

	ftp_error(ERR_WARN, "ls");
	ftp_error(ERR_WARN, args);
	ftp_error(ERR_WARN, tmp);

	if (ftp_validate_path(s->i.root_path, tmp)) {
		if ((rv = ftp_ls_readdir(&response, tmp)) == 1) {
			rv = ftp_send_data(sock, response, ft_strlen(response));
			ft_memdel((void **)&response);
		} else
			rv = ftp_send_data(sock, "Unable to open path", 19);
	} else
		rv = ftp_send_data(sock, "Invalid path", 12);
	ft_memdel((void **)&tmp);
	return (rv);
}

int		ftp_ls_readdir(char **ret, char *tmp)
{
	DIR             *dirp;
	struct dirent   *dir;

	if ((dirp = opendir(tmp)) == NULL) {
		ftp_error(ERR_WARN, "Unable to open path");
		return (0);
	}
	*ret = ft_strdup("-[Server list]-");
	while ((dir = readdir(dirp)) != NULL) {
		if ((ft_strcmp(dir->d_name, ".") != 0) &&
					(ft_strcmp(dir->d_name, "..") != 0)) {
			tmp = ft_strjoinstr(*ret, "\n", dir->d_name);
			ft_memdel((void **)ret);
			*ret = tmp;
		}
	}
	closedir(dirp);
	return (1);
}








int		ftp_ls_validate_path(t_server *s, int sock, char *cwd, char *new_cwd)
{
	char	*tmp;
	int		rp_len;
	int		rv;
	
	(void)cwd;
	rp_len = ft_strlen(s->i.root_path);

// root + ../../

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
}
