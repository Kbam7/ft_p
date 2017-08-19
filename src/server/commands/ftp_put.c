/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_put.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 16:48:48 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/19 09:03:22 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

void	ftp_extract_data(int fd, char (*data)[])
{
	int 	offset;
	int 	rem;
	int		keylen;
	char	*tmp;
	
	rem = ft_strlen(ft_strstr(*data, FTP_DATA_END_KEY));
	keylen = ft_strlen(FTP_DATA_END_KEY);
	offset = ft_strlen(*data) - rem;
	if (offset > 0) {
		tmp = ft_strsub(*data, 0, offset);
ft_fprintf(2, "Found chars before DATA_END_KEY: tmp:\n'%s'\n", tmp);	// debug
		write(fd, tmp, ft_strlen(tmp));
		ft_memdel((void **)&tmp);
	}
	tmp = ft_strsub(*data, offset + keylen, rem - keylen);

ft_fprintf(2, "Found chars after DATA_END_KEY: tmp:\n'%s'\n", tmp);	// debug

	write(fd, tmp, ft_strlen(tmp));
	ft_memdel((void **)&tmp);
}

int		ftp_put_handle_parent(int sock, int fd)
{
	char    data[MAX_DATASIZE + 1];
	int		rv;
	
	ft_memset(data, 0, MAX_DATASIZE + 1);
	if ((rv = ftp_recv_data(sock, &data)) > 0) {
		// save expected file size and check it after file is sent
		//fsize = ft_atoi(data);
		ft_memset(data, 0, rv + 1);
		while ( 1 ) {
			if ((rv = ftp_recv_data(sock, &data)) < 1)
				break;
			if (ft_strstr(data, FTP_DATA_END_KEY))
			{
ft_fprintf(2, "Found end key: data:\n'%s'\n", data);	// debug
				if (ft_strcmp(data, FTP_DATA_END_KEY) != 0) {
					// extract
					ftp_extract_data(fd, &data);
				}
				ftp_error(ERR_INFO, "File received");
				break;
			}
			write(fd, data, rv);
ft_fprintf(2, "data: '%s'\n", data);	// debug
			ft_memset(data, 0, rv + 1);
		}
	}
	return (rv);
}

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
			rv = ftp_put_handle_parent(sock, fds[1]);
			close(fds[1]);
			return (rv);
		}
	}
	return (0);
}

int		ftp_validate_overwrite(int sock)
{
	char    buf[MAX_DATASIZE + 1];

	ft_memset(buf, 0, MAX_DATASIZE + 1);
	ftp_send_data(sock, "overwrite", 9);
	ftp_send_data(sock, "File already exists!\nOverwrite? [yes/no]", 40);
	ftp_recv_data(sock, &buf);
	if (ft_strcmp(buf, "yes") == 0)
		return (1);
	return (-1);
}

int		ftp_put_handle_write(int sock, char *filepath)
{
	int			rv;

	rv = 1;
	if (ftp_file_exists(filepath))
	{
		rv = ftp_validate_overwrite(sock);
	}
	if (rv < 0)
		return (0);
	rv = ftp_put_write(sock, filepath);
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
			if ((rv = ftp_put_handle_write(sock, path)) < 1)
				rv = ftp_send_data(sock, "failed: Unable to put file", 26);
		} else 
			rv = ftp_send_data(sock, "failed: Invalid path", 20);
		ft_memdel((void **)&tmp);
		if (path != NULL)
			ft_memdel((void **)&path);
	}
    return (rv);
}