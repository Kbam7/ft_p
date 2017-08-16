/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_put.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 16:48:48 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/16 12:10:56 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

int     ftp_put(t_server *s, int sock, char *args) {
    ftp_error(ERR_INFO, "Put file onto server");
// SERVER
	// Validate path
	// setup pipe fds
	// Create child process
	// -- CHILD --
	//		Close pipe[1]
	//		dup2(pipe[0], STDIN_FILENO)
	//		run xxd -r (reverse) on filepath
	// -- PARENT --
	//		close pipe[0]
	//		while ((rv = ftp_recv_data(clientSock, buf, MAX_DATASIZE) > 0)
	//			write(pipe[1], buf, rv);

	int		rv;

	rv = -1;
	if (args == NULL) {
		tmp = ftp_get_path(s, args);
		if (ftp_validate_path(s->i.root_path, tmp)) {
			if (chdir(tmp) == 0) {
				ft_memdel((void **)&tmp);
				ftp_get_cwd(&new_cwd);
				if ((tmp = new_cwd + ft_strlen(s->i.root_path)) && *tmp == 0)
					tmp = "/";
				rv = ftp_send_data(sock, tmp, 26);
				ft_memdel((void **)&(s->i.pwd));
				s->i.pwd = ft_strdup(tmp);
				ft_memdel((void **)&new_cwd);
			} else 
				rv = ftp_send_data(sock, "Unable to change directory", 26);
		} else
			ft_memdel((void **)&tmp);
	}
	rv = ftp_send_data(sock, "Invalid path", 12);
    return (rv);


    return (1);
}

pid_t	ftp_init_validation_proc(int (*fds)[2]) {
	pid_t	pid;

	pid = -1;
	if (pipe(*fds) == 0)
		if ((pid = fork()) == -1)
			ftp_error(ERR_WARN, "fork: Unable to validate path");
	return (pid);
}

/* Checks if path is lower than root directory */
int		ftp_validate_path(char *root, char *path)
{
	char	*new_cwd;
	pid_t	pid;
	int		fds[2];
	char	buf[2];

	ft_memset(buf, 0, 2);
	if ((pid = ftp_init_validation_proc(&fds)) == 0) {
		close(fds[0]);
		if (chdir(path) == 0) {
			ftp_get_cwd(&new_cwd);
			if (ftp_validate_cwd(root, new_cwd)) {
				write(fds[1], "1", 1);
				close(fds[1]);
				ft_memdel((void **)&new_cwd);
				exit(EXIT_SUCCESS);
			}
		}
		write(fds[1], "0", 1);
		close(fds[1]);
		exit(EXIT_FAILURE);
	}
	close(fds[1]);
	read(fds[0], buf, 1);
	close(fds[0]);
	if (ft_strcmp(buf, "1") == 0)
		return (1);
	return (0);
}