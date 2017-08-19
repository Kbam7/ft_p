/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_ls.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 10:25:41 by kbamping          #+#    #+#             */
/*   Updated: 2017/08/19 14:17:56 by kbamping         ###   ########.fr       */
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
	execv("/bin/ls", (char * const *)argv);
	ft_putendl("ls failed to execute");
	exit(EXIT_FAILURE);
}

int     ftp_ls(t_server *s, int sock, char *buf)
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

/* int		ftp_ls_readdir(t_server *s, char **ret, char *tmp);

int     ftp_ls(t_server *s, int sock, char *args)
{
	char			*response;
	char			*tmp;
	int				rv;

	rv = 1;
	args = (args == NULL) ? s->i.pwd : args;
	tmp = ftp_get_path(s, args);
	if (ftp_validate_path(s->i.root_path, tmp)) {
		if ((rv = ftp_ls_readdir(s, &response, tmp)) == 1) {
			rv = ftp_send_data(sock, response, ft_strlen(response));
			ft_memdel((void **)&response);
		} else
			rv = ftp_send_data(sock, "Unable to open path", 19);
	} else
		rv = ftp_send_data(sock, "Invalid path", 12);
	ft_memdel((void **)&tmp);
	return (rv);
}

int		ftp_ls_readdir(t_server *s, char **ret, char *tmp)
{
	DIR             *dirp;
	struct dirent   *dir;
	int				root_len;

	if ((dirp = opendir(tmp)) == NULL) {
		ftp_error(ERR_WARN, "Unable to open path");
		return (0);
	}
	root_len = ft_strlen(s->i.root_path);
	*ret = ft_strjoin("-[Server list]- ", tmp + root_len - 1);
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
} */