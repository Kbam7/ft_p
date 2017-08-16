/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_put.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 16:48:48 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/16 15:24:14 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

int     ftp_put(t_server *s, int sock, char *args) {
	ftp_error(ERR_INFO, "Put file onto server");
	(void)sock;
    //(void)args;
	//(void)s;
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

	//int		rv;
	char	*tmp;

	//rv = -1;
	if (args != NULL) {
		tmp = ftp_get_path(s, args);
		if (ftp_validate_path(s->i.root_path, tmp)) {
			if (/* ftp_file_exists(tmp) */ 0) {
				// cannot overwrite file
			} else {
				ftp_error(ERR_INFO, "-here-");
				ftp_error(ERR_INFO, tmp);
			}
		}
		ft_memdel((void **)&tmp);
		ftp_error(ERR_INFO, "invalid");
		ftp_error(ERR_INFO, tmp);
	}
	//rv = ftp_send_data(sock, "Invalid path", 12);
    //return (rv);


    return (1);
}