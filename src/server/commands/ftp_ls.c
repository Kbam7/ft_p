/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_ls.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 10:25:41 by kbamping          #+#    #+#             */
/*   Updated: 2017/08/20 13:00:55 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include <sys/types.h>
#include <dirent.h>

int		ftp_ls_validate_paths(t_server *s, char **argv)
{
	int		i;
	int		rv;
	char	*tmp;

	i = 0;
	rv = 1;
	while (argv[++i] != NULL)
	{
		if (argv[i][0] != '-')
		{
			tmp = ftp_get_path(s, argv[i]);
			if (ftp_validate_path(s->i.root_path, tmp) == NULL)
				rv = 0;
			ft_memdel((void **)&argv[i]);
			argv[i] = tmp;
			ft_memdel((void **)&tmp);
		}
	}
	return (rv);
}

void	ftp_ls_handle_child(t_server *s, int (*fds)[], char *buf)
{
	char	**argv;

	close((*fds)[0]);
	dup2((*fds)[1], STDOUT_FILENO);
	close((*fds)[1]);
	argv = ft_strsplit(buf, ' ');
	if (ftp_ls_validate_paths(s, argv) == 0)
	{
		ft_putendl("Invalid paths detected");
		exit(EXIT_FAILURE);
	}
	execv("/bin/ls", (char *const *)argv);
	ft_putendl("ls failed to execute");
	exit(EXIT_FAILURE);
}

int		ftp_ls(t_server *s, int sock, char *buf)
{
	int		fds[2];
	pid_t	pid;
	char	data[MAX_DATASIZE + 1];

	pid = 0;
	ft_memset(data, 0, MAX_DATASIZE + 1);
	if (pipe(fds) == 0 && (pid = fork()) == 0)
		ftp_ls_handle_child(s, &fds, buf);
	else if (pid > 0)
	{
		close(fds[1]);
		if (ftp_read_fd_write_sock(fds[0], sock) < 1)
			return (ftp_error(ERR_WARN, "failed to send file data"));
	}
	else
		return (0);
	return (pid);
}
