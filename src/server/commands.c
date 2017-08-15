/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 09:59:43 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/15 01:28:12 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include <sys/types.h>
#include <dirent.h>

char	*ftp_cd_get_path(t_server *s, char *args);
int		ftp_cd_validate_cwd(t_server *s, int sock, char *cwd, char *new_cwd);

int     ftp_ls(t_server *s, int sock, char *args)
{
    DIR             *dirp;
    struct dirent   *dir;
	char			*response;
	char			*tmp;
	int				rv;

ftp_error(ERR_FATAL, "ls:28: Need to validate path as done for cd"); 

	args = (args == NULL) ? s->i.pwd : args;
	tmp = ft_strjoin(s->i.root_path, args);
	if ((dirp = opendir(tmp)) == NULL)
	{
		ftp_error(ERR_WARN, "Unable to open path"); 
        rv = ftp_send_data(sock, "Unable to open path", 19);
        return (rv);
    }
	response = ft_strdup("-[Server list]-");
	while ((dir = readdir(dirp)) != NULL)
	{
		if ((ft_strcmp(dir->d_name, ".") != 0) &&
                    (ft_strcmp(dir->d_name, "..") != 0)) {
			tmp = ft_strjoinstr(response, "\n", dir->d_name);
			ft_memdel((void **)&response);
			response = tmp;
		}
	}
    closedir(dirp);
	rv = ftp_send_data(sock, response, ft_strlen(response));
	ft_memdel((void **)&response);
    // ftp_execute_cmd(sock, path, args);
	return (rv);
}

int     ftp_cd(t_server *s, int sock, char *args)
{
	char	*new_cwd;
	char	*cwd;
	char	*tmp;
	int		rv;

ftp_error(ERR_INFO, s->i.root_path);
ftp_error(ERR_INFO, s->i.pwd);
ftp_error(ERR_INFO, args);

	args = (args == NULL) ? s->i.root_path : args;

	cwd = getcwd(NULL, 0);
	tmp = ft_strjoin(cwd, "/");
	ft_memdel((void **)&cwd);
	cwd = tmp;

	tmp = ftp_cd_get_path(s, args);
    if (tmp != NULL && chdir(tmp) == 0) {
		// Get new cwd
		new_cwd = getcwd(NULL, 0);
		ft_memdel((void **)&tmp);
		tmp = ft_strjoin(new_cwd, "/");
		ft_memdel((void **)&new_cwd);
		new_cwd = tmp;

ftp_error(ERR_INFO, new_cwd);

		rv = ftp_cd_validate_cwd(s, sock, cwd, new_cwd);
		ft_memdel((void **)&new_cwd);
	} else 
		rv = ftp_send_data(sock, "Unable to change directory", 26);
	ft_memdel((void **)&cwd);
    return (rv);
}

int		ftp_cd_validate_cwd(t_server *s, int sock, char *cwd, char *new_cwd)
{
	char	*tmp;
	int		rp_len;
	int		rv;
	
	(void)cwd;
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
}

char	*ftp_cd_get_path(t_server *s, char *args)
{
	char	*tmp;
	
	// root
 	if (ft_strcmp(args, "/") == 0)
		tmp = ft_strdup(s->i.root_path);
	else if (args[0] == '/') // absolute
		tmp = ft_strjoin(s->i.root_path, args + 1);
	else // relative to pwd
		tmp = ft_strjoinstr(s->i.root_path, s->i.pwd, args);
	return (tmp);
}

int     ftp_get(t_server *s, int sock, char *args)
{
    ftp_error(ERR_INFO, "Get file from server");
    // Get file on server
    (void)sock;
    (void)args;
	(void)s;
    return (1);
}

int     ftp_put(t_server *s, int sock, char *args)
{
    ftp_error(ERR_INFO, "Put file onto server");
    // Put file on server
    (void)sock;
    (void)args;
	(void)s;
    return (1);
}

int     ftp_pwd(t_server *s, int sock, char *args)
{
	int	rv;
	
	if (args)
		args = NULL;
	rv = ftp_send_data(sock, s->i.pwd, ft_strlen(s->i.pwd));
	return (rv);
}

int     ftp_quit(void)
{
    ftp_error(ERR_INFO, "User shutdown client");
    return (0);
}