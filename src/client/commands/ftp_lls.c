/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_lls.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/17 08:13:58 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/20 12:07:00 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int		ftp_lls(char *cmd)
{
	pid_t	pid;
	int		fds[2];
	char	*buf;

	pid = 0;
	if (pipe(fds) == 0 && (pid = fork()) == 0)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		execv("/bin/ls", (char *const *)ft_strsplit(cmd, ' '));
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
	return (1);
}
