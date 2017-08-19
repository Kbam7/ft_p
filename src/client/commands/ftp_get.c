/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/17 08:13:58 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/19 16:35:05 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int		ftp_get_write(int sock, char *filepath)
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
			close(fds[0]);
			rv = ftp_write_from_socket(sock, fds[1]);
			close(fds[1]);
			return (rv);
		}
	}
	return (0);
}

int		ftp_validate_overwrite(void)
{
	char	*line;
	int		rv;

	rv = 0;
	ftp_error(ERR_WARN, "File already exists!\nOverwrite? [yes/no]");
	if (ft_gnl(STDIN_FILENO, &line) > 0) {
		if (ft_strcmp(line, "yes") == 0)
			rv = 1;
		ft_memdel((void **)&line);
	}
	return (rv);
}

int     ftp_get(int sock, char *cmd) {
	int		rv;
	char	data[MAX_DATASIZE + 1];

	rv = 1;
	if (!ftp_file_exists(cmd + 4) || ftp_validate_overwrite())
	{
		if ((rv = ftp_send_data(sock, cmd, ft_strlen(cmd))) > 0)
		{
			ft_memset(data, 0, MAX_DATASIZE + 1);
			if ((rv = ftp_recv_data(sock, &data)) > 0)
			{
				if (ft_strcmp(data, "writing") == 0)
					if ((rv = ftp_get_write(sock, cmd + 4)) > 0)
						ftp_error(ERR_INFO, "File received");
				if (ft_strncmp(data, "failed: ", 8) == 0)
					ftp_error(ERR_WARN, data);
			}
		}
	}
	return (rv);
}