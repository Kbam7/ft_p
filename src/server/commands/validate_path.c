/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 09:59:43 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/16 12:17:26 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

int		ftp_validate_cwd(char *root, char *new_cwd)
{
	if (ft_strncmp(new_cwd, root, ft_strlen(root)) == 0) 
		return (1);
	return (0);
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
int		ftp_validate_path(char *type, char *root, char *path)
{
	char	*new_cwd;
	pid_t	pid;
	int		fds[2];
	char	buf[2];

	if (ft_strcmp(type, "dir"))
		ftp_validate_dir_path();
	else if (ft_strcmp(type, "file"))
		ftp_validate_file_path();
	else
		return (0);
	
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