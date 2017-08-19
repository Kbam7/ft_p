/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_mput.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/17 08:13:58 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/19 18:19:50 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int		ftp_mget_all()
{
	pid_t	pid;
	int		fds[2];
	char	**argv;
	char	*buf;

	pid = 0;
	if (pipe(fds) == 0 && (pid = fork()) == 0)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		argv = ft_strsplit(cmd, ' ');
		execv("/bin/ls", (char * const *)argv);
		ft_putendl("ls failed to execute");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		close(fds[1]);
		while (ft_gnl(fds[0], &buf) > 0)
		{
			ft_putendl(buf);
			ft_memdel((void **)&buf);
		}
		close(fds[0]);
}

int     ftp_mput(int sock, char *cmd)
{
	t_split_string	sp;
	size_t			i;
	int				rv;
	char			*tmp;

	rv = 1;
	sp = ft_nstrsplit(cmd, ' ');
	if (sp.words > 1)
	{
		i = 0;
		while (rv && ++i < sp.words)
		{
			if (ft_strcmp(sp.strings[i], "*") == 0)
				ftp_mget_all();
			else {
				tmp = ft_strjoin("put ", sp.strings[i]);
				if (ftp_put(sock, tmp) < 1)
					rv = 0;
				ft_memdel((void **)&tmp);
			}
		}
	}
	ft_free_split(&sp);
	return (rv);
}