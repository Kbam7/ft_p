/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 09:59:43 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/14 12:23:46 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int     ftp_ls(int sock, char *cmd)
{
    char    buf[MAX_MSGSIZE];
    char    *str;
    char    *tmp;

    ftp_error(ERR_INFO, "Doing server ls");
    // List files on server
    ftp_send_msg(sock, cmd, ft_strlen(cmd));
    while (ftp_receive_msg(sock, &buf) > 0)
    {
        tmp = ft_strjoin(str, buf);
        ft_memdel((void **)&str);
        str = tmp;
    }
    ft_putendl(str);
    return (1);
}

int     ftp_cd(int sock, char *cmd)
{
    ftp_error(ERR_INFO, "Doing server cd");
    // Change directory on server
    ftp_send_msg(sock, cmd, ft_strlen(cmd));
    return (1);
}

int     ftp_get(int sock, char *cmd)
{
    ftp_error(ERR_INFO, "Get file from server");
    // Get file on server
    ftp_send_msg(sock, cmd, ft_strlen(cmd));
    return (1);
}

int     ftp_put(int sock, char *cmd)
{
    ftp_error(ERR_INFO, "Put file onto server");
    // Put file on server
    ftp_send_msg(sock, cmd, ft_strlen(cmd));
    return (1);
}

int     ftp_pwd(int sock, char *cmd)
{
    ftp_error(ERR_INFO, "Display current server directory");
    // Display pwd on server
    ftp_send_msg(sock, cmd, ft_strlen(cmd));
    return (1);
}

int     ftp_quit(int sock)
{
    ftp_error(ERR_INFO, "Quitting");
    ftp_send_msg(sock, "quit", 4);
    return (1);
}