/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_put.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 16:48:48 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/18 15:07:41 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

int		ftp_put_write(int sock, char *filepath)
{
	int		fds[2];
	char    data[MAX_DATASIZE + 1];
	pid_t	pid;
	int		rv;
	int read = 1;
	//struct stat	st;
	off_t	fsize;


	if (pipe(fds) == 0) {
		if ((pid = fork()) == 0) {
			close(fds[1]);
			dup2(fds[0], STDIN_FILENO); // read from pipe
			//dup2(fd, STDOUT_FILENO); // STDOUT goes to file
			close(fds[0]);
			execl("/usr/bin/xxd", "xxd", "-r", "-", filepath, (char *)NULL);
			exit(EXIT_FAILURE);
		}  else if (pid > 0) {
			ftp_send_data(sock, "writing", 7);


// NEED TO SEND FILESIZE FIRST. Otherside expects that much data and stops reading
// when the new file it created is at the same size as the one on the client
			close(fds[0]);
			ft_memset(data, 0, MAX_DATASIZE + 1);
			if ((rv = ftp_recv_data(sock, &data)) > 0) {

//ft_printf("size receieved: '%s'\n", data);	// debug

			fsize = ft_atoi(data);
			//fsize = 5014;

//ft_printf("size cnvtd1: '%i'\n", fsize);	// debug

				ft_memset(data, 0, rv + 1);
			
//ft_printf("size cnvtd2: '%i'\n", fsize);	// debug

ft_fprintf(2, "reading hex data from client: start\n");	// debug

				while ( read ) {
					if ((rv = ftp_recv_data(sock, &data)) < 1)
						break;
				//ft_printf("rv: '%d'    data_len: '%d'\n", rv, ft_strlen(data));	// debug
					ft_fprintf(2, "Received data:\n'%s'\n", data);	// debug


					/* if ((rv = send(sock, "1", 1, 0)) < 1)
						return (rv); */

					if (ft_strstr(data, FTP_DATA_END_KEY))
					{
						char	*tmp;
						int len = ft_strlen(FTP_DATA_END_KEY);

						//ft_printf("1-found FTP_DATA_END_KEY: len: '%d'  rv: '%d'  data:'%s'\n", len, rv, data); // debug

						if (ft_strcmp(data, FTP_DATA_END_KEY) != 0/* rv > len */) {
							rv = ft_strlen(data);
							tmp = ft_strsub(data, len, rv - len);

				//ft_printf("found excess data: tmp: '%s'\n", tmp);	// debug

							/* ft_memset(data, 0, rv + 1);
							ft_memcpy(data, data + len, rv - len); */

				ft_fprintf(2, "writing excess data to pipe: data:'%s'\n", tmp); // debug

							write(fds[1], tmp, ft_strlen(tmp));
							ft_memdel((void **)&tmp);
						}
						break;
					}

//ft_fprintf(2, "writing data to pipe: rv: '%i'\n'%s'\n", rv, data); // debug
					write(fds[1], data, rv);
ft_fprintf(2, "finished writing data to pipe\n");	// debug
					ft_memset(data, 0, rv + 1);


				}
			}
			close(fds[1]);
ft_fprintf(2, "reading hex data from client: end\n");	// debug
			return (rv);
		}
	}
	return (0);
}

int		ftp_validate_overwrite(int sock/* , char *filepath */)
{
	char    buf[MAX_DATASIZE + 1];

//ft_printf("validate_overwrite: path: %s\n", filepath); // debug

	ft_memset(buf, 0, MAX_DATASIZE + 1);

//ft_printf("validate_overwrite: path: %s\n", filepath); // debug

	ftp_send_data(sock, "overwrite", 9);
	ftp_send_data(sock, "File already exists!\nOverwrite? [yes/no]", 40);
//ft_printf("validate_overwrite: path: %s\n", filepath); // debug
	ftp_recv_data(sock, &buf);
//ft_printf("validate_overwrite: path: %s\n", filepath); // debug
	if (ft_strcmp(buf, "yes") == 0) {
		//ft_printf("validate_overwrite: path: %s\n", filepath); // debug
		return (1/* open(filepath, O_CREAT | O_TRUNC, 0777) */);
	}
	else
		return (-1);
}

int		ftp_put_handle_write(int sock, char *filepath)
{
	//int			fd;
	int			rv;

	rv = 1;
	if (ftp_file_exists(filepath))
	{
		rv = ftp_validate_overwrite(sock/* , filepath */);
	}/*  else
		fd = open(filepath, O_CREAT | O_TRUNC, 0777); */
	if (rv < 0)
		return (0);
	rv = ftp_put_write(sock, filepath);
	/* close(fd); */
	return (rv);
}

int     ftp_put(t_server *s, int sock, char *args) {
	ftp_error(ERR_INFO, "Put file onto server");
    //(void)args;
	//(void)s;
// SERVER
	// Validate path
	//	check if file exists, prompt user to overwrite
	// setup pipe fds
	// Create child process
	// -- CHILD --
	//		Close pipe[1]
	//		dup2(pipe[0], STDIN_FILENO) // read from pipe
	//		dup2(new_fd, STDOUT_FILENO) // STDOUT goes to file
	//		run xxd -r - filepath (reverse) on filepath
	// -- PARENT --
	//		close pipe[0]
	//		while ((rv = ftp_recv_data(clientSock, buf, MAX_DATASIZE) > 0)
	//			write(pipe[1], buf, rv);

	//int		rv;
	char	*tmp;
	char	*path;
	int		rv;

	rv = 1;
	if (args != NULL) {
		ft_putendl(args); 				// debug
		tmp = ftp_get_path(s, args);
		ft_putendl(tmp); 				// debug
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