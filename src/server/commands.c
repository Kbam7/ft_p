/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 09:59:43 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/14 12:27:46 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include <sys/types.h>
#include <dirent.h>

int     ftp_ls(int sock, char *args)
{
    DIR             *dirp;
    struct dirent   *dir;
    
    ftp_error(ERR_INFO, "Doing server ls");
    // List files on server
    if ((dirp = opendir(args)) == NULL) {
        ftp_error(ERR_WARN, "Unable to open path"); 
        ftp_send_msg(sock, "Unable to open path", 19); 
        return (0);
    }
    while ((dir = readdir(dirp)) != NULL)
    {
        if ((ft_strcmp(dir->d_name, ".") != 0) &&
                    (ft_strcmp(dir->d_name, "..") != 0)) {
            ftp_error(ERR_INFO, ft_strjoin("Found dir->d_name", dir->d_name));
            ftp_send_msg(sock, dir->d_name, ft_strlen(dir->d_name));
        }
    }
    closedir(dirp);
    // ftp_execute_cmd(sock, path, args);
    return (1);
}

int     ftp_cd(int sock, char *args)
{
    ftp_error(ERR_INFO, "Doing server cd");
    // Change directory on server
    (void)sock;
    (void)args;
    return (1);
}

int     ftp_get(int sock, char *args)
{
    ftp_error(ERR_INFO, "Get file from server");
    // Get file on server
    (void)sock;
    (void)args;
    return (1);
}

int     ftp_put(int sock, char *args)
{
    ftp_error(ERR_INFO, "Put file onto server");
    // Put file on server
    (void)sock;
    (void)args;
    return (1);
}

int     ftp_pwd(int sock, char *args)
{
    ftp_error(ERR_INFO, "Display current server directory");
    // Display pwd on server
    (void)sock;
    (void)args;
    return (1);
}

int     ftp_quit(void)
{
    ftp_error(ERR_INFO, "Quitting");
    return (1);
}