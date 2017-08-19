/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_put.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 16:48:48 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/19 17:02:25 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

int		ftp_put_write(int sock, char *filepath)
{
	int		fds[2];
	pid_t	pid;
	int		rv;

	if (pipe(fds) == 0) {
		if ((pid = fork()) == 0) {
			close(fds[1]);
			dup2(fds[0], STDIN_FILENO);
			close(fds[0]);
			execl("/usr/bin/xxd", "xxd", "-r", "-", filepath, (char *)NULL);
			exit(EXIT_FAILURE);
		}  else if (pid > 0) {
			ftp_send_data(sock, "writing", 7);
			close(fds[0]);
			rv = ftp_write_from_socket(sock, fds[1]);
			close(fds[1]);
			return (rv);
		}
	}
	return (0);
}

int		ftp_validate_overwrite(int sock)
{
	char    buf[MAX_DATASIZE + 1];

	ftp_error(ERR_INFO, "overwrite");
	ft_memset(buf, 0, MAX_DATASIZE + 1);
	if (ftp_send_data(sock, "overwrite", 9) > 0)
	{
		if (ftp_recv_data(sock, &buf) > 0)
		{
			if (ft_strcmp(buf, "yes") == 0)
				return (1);
			ftp_send_data(sock, "failed: Overwrite cancelled", 27);
			return (0);
		}
	}
	return (-1);
}

int		ftp_put_handle_write(int sock, char *filepath)
{
	int			rv;

	rv = 1;
	if (ftp_file_exists(filepath))
		rv = ftp_validate_overwrite(sock);
	if (rv > 0)
	{
		if ((rv = ftp_put_write(sock, filepath)) > 0)
		ftp_error(ERR_INFO, "File received");
	}
	else if (rv == 0)
		rv = 1;
	return (rv);
}

int     ftp_put(t_server *s, int sock, char *args) {
	char	*tmp;
	char	*path;
	int		rv;

	rv = 1;
	if (args != NULL) {
		tmp = ftp_get_path(s, args);
		if ((path = ft_strdup(ftp_validate_path(s->i.root_path, tmp))) != NULL) {
			if ((rv = ftp_put_handle_write(sock, path)) < 0)
				rv = ftp_send_data(sock, "failed: Unable to put file", 26);
		} else 
			rv = ftp_send_data(sock, "failed: Invalid path", 20);
		ft_memdel((void **)&tmp);
		if (path != NULL)
			ft_memdel((void **)&path);
	}
    return (rv);
}