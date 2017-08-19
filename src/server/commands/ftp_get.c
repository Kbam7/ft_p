/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 16:48:48 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/19 16:42:57 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

int		ftp_get_handle_write(int sock, char *path)
{
	int		fds[2];
	pid_t	pid;
	struct stat	st;

	if (lstat(path, &st) == 0 && pipe(fds) == 0) {
		if ((pid = fork()) == 0) {
			close(fds[0]);
			dup2(fds[1], STDOUT_FILENO);
			close(fds[1]);
			execl("/usr/bin/xxd", "xxd", path, (char *)NULL);
			exit(EXIT_FAILURE);
		}  else if (pid > 0) {
			ftp_send_data(sock, "writing", 7);
			close(fds[1]);
			if (ftp_read_fd_write_sock(fds[0], sock) > 0 && close(fds[0]) < 1)
				return (1);
			close(fds[0]);
		}
	}
	return (0);
}

int     ftp_get(t_server *s, int sock, char *args)
{
	int		rv;
	char	*path;
	char	*tmp;

	if (args != NULL)
	{
		tmp = ftp_get_path(s, args);
		path = ft_strdup(ftp_validate_path(s->i.root_path, tmp));
		if (path != NULL)
		{
			if (ftp_is_reglr_file(path) || ftp_is_symlink(path))
			{
				if ((rv = ftp_get_handle_write(sock, path)) < 1)
					rv = ftp_send_data(sock, "failed: Unable to get file", 26);
				else
					ftp_error(ERR_INFO, "File sent");
			}
			else
				rv = ftp_send_data(sock, "failed: Unsupported file type", 26);
		}
		else 
			rv = ftp_send_data(sock, "failed: Invalid path", 20);
		ft_memdel((void **)&tmp);
		if (path != NULL)
			ft_memdel((void **)&path);
	}
	else
		rv = ftp_send_data(sock, "failed: No path found", 13);
    return (rv);
}