/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 09:59:43 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/15 14:31:28 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

int		ftp_validate_cwd(char *root, char *new_cwd);

void	ftp_get_cwd(char **cwd)
{
	char	*tmp;
	
	// Get cwd, add '/' to end
	*cwd = getcwd(NULL, 0);
	tmp = ft_strjoin(*cwd, "/");
	ft_memdel((void **)cwd);
	*cwd = tmp;
}

char	*ftp_get_path(t_server *s, char *args)
{
	char	*tmp;
	
	// root
 	if (ft_strcmp(args, "/") == 0)
		tmp = ft_strdup(s->i.root_path);
	else if (args[0] == '/') // absolute
		tmp = ft_strjoin(s->i.root_path, args + 1);
	else // relative to pwd
		tmp = ft_strjoinstr(s->i.root_path, s->i.pwd, args);
	return (tmp);
}

/* Checks if path is lower than root directory */
int		ftp_validate_path(char *root, char *path)
{
	char	*new_cwd;
	int		rv;
	pid_t	pid;
	int		fds[2];
	char	buf[2];

	ft_memset(buf, 0, 2);
	if (pipe(fds) == 0) {
		if ((pid = fork()) == -1)
			return (!ftp_error(ERR_WARN, "fork: Unable to validate path"));
		else if (pid == 0) {
			close(fds[0]);
			if (chdir(path) == 0) {
				ftp_get_cwd(&new_cwd);
				rv = ftp_validate_cwd(root, new_cwd);
				ft_memdel((void **)&new_cwd);
				if (rv) {
					ftp_error(ERR_WARN, "vp: Child exiting successfully");
					write(fds[1], "1", 1);
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
		ftp_error(ERR_WARN, buf);
		if (ft_strcmp(buf, "1") == 0)
			return (1);
	}
	return (0);
}

int		ftp_validate_cwd(char *root, char *new_cwd)
{
	if (ft_strncmp(new_cwd, root, ft_strlen(root)) == 0) 
		return (1);
	return (0);
}


/*
int		ftp_cd_validate_cwd(t_server *s, int sock, char *cwd, char *new_cwd)
{
	char	*tmp;
	int		rp_len;
	int		rv;
	
	rp_len = ft_strlen(s->i.root_path);
	if (ft_strncmp(new_cwd, s->i.root_path, rp_len) == 0) {
		// new cwd contains atleast the root dir
		if ((rv = ftp_send_data(sock, "Success! Directory changed", 26)) < 1)
			return (rv);
		// get ptr to begining of path, relative to root_path
		tmp = new_cwd + rp_len;
		ft_memdel((void **)&s->i.pwd);
		s->i.pwd = ft_strdup(((*tmp == '\0') ? "/" : tmp));
	} else if (chdir(cwd) == 0) {
		if ((rv = ftp_send_data(sock, "Unable to change directory", 26)) < 1)
			return (rv);
	} else {
		ftp_send_data(sock, UNAUTHORISED_AREA, ft_strlen(UNAUTHORISED_AREA));
		ftp_error(ERR_FATAL, "cd: unauthorised path accessed, killing client");
	}
	return (1);
}
*/


int     ftp_get(t_server *s, int sock, char *args)
{
    ftp_error(ERR_INFO, "Get file from server");
    // Get file on server
    (void)sock;
    (void)args;
	(void)s;
    return (1);
}

int     ftp_put(t_server *s, int sock, char *args)
{
    ftp_error(ERR_INFO, "Put file onto server");
    // Put file on server
    (void)sock;
    (void)args;
	(void)s;
    return (1);
}

int     ftp_pwd(t_server *s, int sock, char *args)
{
	int	rv;
	
	if (args)
		args = NULL;
	rv = ftp_send_data(sock, s->i.pwd, ft_strlen(s->i.pwd));
	return (rv);
}

int     ftp_quit(void)
{
    ftp_error(ERR_INFO, "User shutdown client");
    return (0);
}