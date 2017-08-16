/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 09:59:43 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/16 14:45:47 by kbamping         ###   ########.fr       */
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

char	*ftp_remove_filename(char *path)
{
	char	*tmp;
	int 	len;

	ftp_error(ERR_INFO, ft_strjoin("OLD:", path));
	len = ft_strlen(path);
	tmp = ft_strrchr(path, '/');
	len = len - ft_strlen(tmp);
	while (path[++len])
		path[len] = 0;
	ftp_error(ERR_INFO, ft_strjoin("NEW:", path));
	return (path);
}

int		ftp_handle_child(char *type, char *root, char *path)
{
	char	*new_cwd;
	
	if (ft_strcmp(type, "file") == 0)
		path = ftp_remove_filename(path);
	if (chdir(path) == 0)
	{
		ftp_get_cwd(&new_cwd);
		if (ftp_validate_cwd(root, new_cwd)) {
			ft_memdel((void **)&new_cwd);
			return (1);
		}
		if (new_cwd)
			ft_memdel((void **)&new_cwd);
	}
	return (0);
}

/* Checks if path is lower than root directory */
int		ftp_validate_path(char *type, char *root, char *path)
{
	char	buf[4096];

	(void)type;
	ft_memset(buf, 0, 4096);
	ft_putendl(path);
	realpath(path, buf);
	ft_putendl(buf);
	if (ft_strncmp(buf, root, ft_strlen(root) - 1) == 0) 
		return (1);
	return (0);
}


/* {
	pid_t	pid;
	int		fds[2];
	char	buf[2];
	
	ft_memset(buf, 0, 2);
	if ((pid = ftp_init_validation_proc(&fds)) == 0) {
		close(fds[0]);
		if (ftp_handle_child(type, root, path)) {
			write(fds[1], "1", 1);
		} else
			write(fds[1], "0", 1);
		close(fds[1]);
		exit(EXIT_SUCCESS);
	}
	close(fds[1]);
	read(fds[0], buf, 1);
	close(fds[0]);
	if (ft_strcmp(buf, "1") == 0)
		return (1);
	return (0);
} */