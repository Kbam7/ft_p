/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 10:02:54 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/14 11:21:01 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int		ftp_handle_user_commands(int sock)
{
	char	buf[MAX_MSGSIZE + 1];
	int		rv;
	char	*tmp;
	
    // Init array
    ft_memset(buf, 0, MAX_MSGSIZE + 1);
    // Get input from keyboard
    while ((rv = ft_gnl(STDIN_FILENO, &tmp)) == 0)
        ;
    if (rv < 0)
        return (-1);
	// Truncate buf to max size
    if (rv > MAX_MSGSIZE)
        tmp[MAX_MSGSIZE] = 0;

    ft_memcpy(buf, tmp, ft_strlen(tmp));
    ft_memdel((void *)&tmp);
    
    ftp_error(ERR_INFO, buf);
    ftp_error(ERR_INFO, ft_itoa(ft_strlen(buf)));

    return (ftp_run_command(sock, buf));


/*         // Send user input to server
    if (ftp_send_msg(sockfd, buf) < 1)
        break;

    // Read response
    ft_memset(buf, 0, MAX_MSGSIZE + 1);
    if ((n = ftp_receive_msg(sockfd, &buf)) == 0) // 0 Bytes from server... ? Disconnected?
        break;

    // If there is a response, print out response
    if (n > 0)
    {
        printf("client: received '%s'\n",buf);
        if (strcmp(buf, "quit") == 0)
            break;
    } */
}

int		ftp_run_command(int sock, char *buf)
{
    int     rv;
    char    *full_cmd;
	char    *cmd;
    
    rv = 0;
    full_cmd = ftp_build_command(buf, &cmd);

    if (ft_strcmp(cmd, "ls") == 0)
        rv = ftp_ls(sock, full_cmd);
    else if (ft_strcmp(cmd, "cd") == 0)
        rv = ftp_cd(sock, full_cmd);
    else if (ft_strcmp(cmd, "get") == 0)
        rv = ftp_get(sock, full_cmd);
    else if (ft_strcmp(cmd, "put") == 0)
        rv = ftp_put(sock, full_cmd);
    else if (ft_strcmp(cmd, "pwd") == 0)
        rv = ftp_pwd(sock, full_cmd);
    else if (ft_strcmp(cmd, "quit") == 0)
        rv = ftp_quit(sock);
	else
		ftp_error(ERR_WARN, "Unrecognised command");
    ft_memdel((void **)&full_cmd);
	ft_memdel((void **)&cmd);
    return (rv);
}

char    *ftp_build_command(char *buf, char **cmd)
{
    t_split_string  sp;
    char            *ret;
    
    sp = ft_nstrsplit(buf, ' ');
    *cmd = ft_strdup(sp.strings[0]);
    if (sp.words < 2)
        ret = ft_strdup(sp.strings[0]);
    else
        ret = ft_strjoinstr(*cmd, " ", sp.strings[1]);
    ft_free_split(&sp);
    return (ret);
}