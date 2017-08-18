/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_put.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/17 08:13:58 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/18 12:12:55 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int		ftp_put_write_parent(int sock, int fd, off_t fsize, char (*data)[])
{
	int		rv;
	char	*size;

ftp_error(ERR_INFO, "write_parent - strt"); // debug
	rv = 0;
	size = ft_itoa(fsize);

ft_printf("sending size: size: '%s'\n", size);	// debug

	if (ftp_send_data(sock, size, ft_strlen(size)) > 1) {
		while ((rv = read(fd, data, MAX_DATASIZE)) > 0) {

	ftp_error(ERR_INFO, "write_parent - reading"); // debug

			if ((rv = ftp_send_data(sock, *data, rv)) < 1)
				break;

	ftp_error(ERR_INFO, "write_parent - data sent"); // debug

			ft_memset(*data, 0, rv + 1);
		}
	}
	ft_memdel((void **)&size);
	close(fd);
	if (rv < 0) {
		ft_memset(*data, 0, 25);
		ft_memcpy(*data, "failed to send file data", 24);
	}
	ftp_send_data(sock, FTP_DATA_END, ft_strlen(FTP_DATA_END));
ftp_error(ERR_INFO, "write_parent - end"); // debug
	return (rv);
}

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
			return (ftp_put_write_parent(sock, fds[0], st.st_size, data));
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

	ft_memset(*data, 0, MAX_DATASIZE + 1);
	if ((rv2 = ftp_recv_data(sock, data)) < 1)
		return (rv2);
	ft_putendl(*data);
	if (ft_gnl(STDIN_FILENO, &line) > 0) {;
		rv = (ft_strcmp(line, "yes") == 0) ? 1 : 0;
		rv2 = ftp_send_data(sock, line, ft_strlen(line));
		ft_memdel((void **)&line);
		if (rv2 < 1)
			return (rv2);
	}
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

ft_printf("receiving initial response\n");	// debug

			if ((rv = ftp_recv_data(sock, &data)) < 1)
				return (rv);

ft_printf("received : '%s'\n", data); // debug

			if (ft_strncmp(data, "overwrite", 9) == 0)
				rv = ftp_put_confirm_overwrite(sock, &data);
			if (rv > 0 && ft_strncmp(data, "writing", 7) == 0)
				rv = ftp_put_handle_write(sock, cmd + 4, &data);
			if (rv < 1 || ft_strncmp(data, "failed", 6) == 0)
				ftp_error(ERR_WARN, data);
		} else 
			ftp_error(ERR_WARN, "File type not supported");
	} else
		ftp_error(ERR_WARN, "File not found");
	return (rv);
}