/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 10:02:54 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/19 13:11:46 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

char    *ftp_build_command(char *buf, char **cmd)
{
    t_split_string  sp;
	char            *ret;
	char            *tmp;
	size_t			i;

	i = 0;
	sp = ft_nstrsplit(buf, ' ');
	*cmd = ft_strdup(sp.strings[0]);
    if (sp.words < 2)
        ret = ft_strdup(sp.strings[0]);
    else {
		ret = ft_strdup(sp.strings[0]);
		while (++i < sp.words && sp.strings[i] != NULL)
		{
			tmp = ft_strjoinstr(ret, " ", sp.strings[i]);
			ft_memdel((void **)&ret);
			ret = tmp;
		}
	}
    ft_free_split(&sp);
    return (ret);
}

int		ftp_run_command(t_session *s,  char *buf)
{
    int     rv;
    char    *full_cmd;
	char    *cmd;
    
    rv = 0;
    full_cmd = ftp_build_command(buf, &cmd);

    if (ft_strcmp(cmd, "ls") == 0)
        rv = ftp_ls(s->socket, full_cmd);
    else if (ft_strcmp(cmd, "cd") == 0)
        rv = ftp_cd(s->socket, full_cmd);
    else if (ft_strcmp(cmd, "get") == 0)
        rv = ftp_get(s->socket, full_cmd);
    else if (ft_strcmp(cmd, "put") == 0)
        rv = ftp_put(s->socket, full_cmd);
    else if (ft_strcmp(cmd, "pwd") == 0)
        rv = ftp_pwd(s->socket, full_cmd);
    else if (ft_strcmp(cmd, "quit") == 0)
        rv = ftp_quit(s->socket);
	else if (ft_strcmp(cmd, "menu") == 0 || ft_strcmp(cmd, "help") == 0)
		rv = ftp_print_client_menu();
	else if (ft_strcmp(cmd, "clear") == 0)
		rv = ft_printf("\033[H\033[J");
	else
		ftp_error(ERR_WARN, "Unrecognised command");
    ft_memdel((void **)&full_cmd);
	ft_memdel((void **)&cmd);
    return (rv);
}

int		ftp_handle_user_commands(t_session *s)
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
	// Truncate tmp to buf size
    if (rv > MAX_MSGSIZE)
        tmp[MAX_MSGSIZE] = 0;

    ft_memcpy(buf, tmp, ft_strlen(tmp));
    ft_memdel((void *)&tmp);

    return (ftp_run_command(s, buf));
}