/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 09:59:43 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/14 16:01:47 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include <sys/types.h>
#include <dirent.h>

int     ftp_ls(t_server *s, int sock, char *args)
{
    DIR             *dirp;
    struct dirent   *dir;
	char			*response;
	char			*tmp;

	args = (args == NULL) ? s->i.pwd : args;
	tmp = ft_strjoin(s->i.root_path, args);
	if ((dirp = opendir(tmp)) == NULL)
	{
		ftp_error(ERR_WARN, "Unable to open path"); 
        ftp_send_msg(sock, "Unable to open path", 19); 
        return (0);
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
	ftp_send_msg(sock, response, ft_strlen(response));
    // ftp_execute_cmd(sock, path, args);
	return (1);
}

int     ftp_cd(t_server *s, int sock, char *args)
{
    ftp_error(ERR_INFO, "Doing server cd");
    // Change directory on server
    (void)sock;
    (void)args;
	(void)s;
    return (1);
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
    ftp_error(ERR_INFO, "Display current server directory");
    // Display pwd on server
    (void)sock;
    (void)args;
	(void)s;
	return (1);
}

int     ftp_quit(void)
{
    ftp_error(ERR_INFO, "Quitting");
    return (1);
}