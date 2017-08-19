/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_put.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/17 08:13:58 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/19 16:46:23 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int		ftp_put_handle_write(int sock, char *path, char (*data)[])
{
	int		fds[2];
	pid_t	pid;
	struct stat	st;

	if (lstat(path, &st) == 0 && pipe(fds) == 0) {
		ft_memset(*data, 0, MAX_DATASIZE + 1);
		if ((pid = fork()) == 0) {
			close(fds[0]);
			dup2(fds[1], STDOUT_FILENO);
			close(fds[1]);
			execl("/usr/bin/xxd", "xxd", path, (char *)NULL);
			exit(EXIT_FAILURE);
		}  else if (pid > 0) {
			close(fds[1]);
			if (ftp_read_fd_write_sock(fds[0], sock) > 0 && close(fds[0]) < 1)
				return (1);
			close(fds[0]);
		}
	}
	ft_memset(*data, 0, 25);
	ft_strcpy(*data, "failed to send file data");
	return (0);
}

int		ftp_put_confirm_overwrite(int sock, char (*data)[])
{
	char	*line;
	int		rv;
	int		rv2;

	ftp_error(ERR_INFO, "File already exists!\nOverwrite? [yes/no]");
	ft_memset(*data, 0, MAX_DATASIZE + 1);
/* 	if ((rv2 = ftp_recv_data(sock, data)) < 1)
		return (rv2);
	ft_putendl(*data); */
	if (ft_gnl(STDIN_FILENO, &line) > 0) {
		rv = (ft_strcmp(line, "yes") == 0) ? 1 : 0;
		rv2 = ftp_send_data(sock, line, ft_strlen(line));
		ft_memdel((void **)&line);
		if (rv2 < 1)
			return (rv2);
	}
	else
		ftp_send_data(sock, "no", 2);
	ft_memset(*data, 0, MAX_DATASIZE + 1);
	if ((rv2 = ftp_recv_data(sock, data)) < 1)
		return (rv2);
	return (rv);
}

int     ftp_put(int sock, char *cmd)
{
	char    data[MAX_DATASIZE + 1];
	int rv;

	rv = 1;
	if (ftp_file_exists(cmd + 4)) {
		if (ftp_is_reglr_file(cmd + 4) || ftp_is_symlink(cmd + 4)) {
			ft_memset(data, 0, MAX_DATASIZE + 1);
			if ((rv = ftp_send_data(sock, cmd, ft_strlen(cmd))) < 1)
				return (rv);
			if ((rv = ftp_recv_data(sock, &data)) < 1)
				return (rv);
			if (ft_strncmp(data, "overwrite", 9) == 0)
				rv = ftp_put_confirm_overwrite(sock, &data);
			if (rv > 0 && ft_strncmp(data, "writing", 7) == 0)
				rv = ftp_put_handle_write(sock, cmd + 4, &data);
			if (rv < 1 || ft_strncmp(data, "failed", 6) == 0)
				ftp_error(ERR_WARN, data);
			else
				ftp_error(ERR_INFO, "File sent");
		} else 
			ftp_error(ERR_WARN, "File type not supported");
	} else
		ftp_error(ERR_WARN, "File not found");
	return (rv);
}