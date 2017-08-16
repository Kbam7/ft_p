/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 09:59:43 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/16 12:04:04 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int     ftp_ls(int sock, char *cmd)
{
    char    data[MAX_DATASIZE + 1];
	int rv;

	ft_memset(data, 0, MAX_DATASIZE + 1);
    // Send command
    if ((rv = ftp_send_data(sock, cmd, ft_strlen(cmd))) < 1)
		return (rv);
	// Read and print out response
	if ((rv = ftp_recv_data(sock, &data)) < 1)
		return (rv);
	ft_putendl(data);
	return (1);
}

int     ftp_cd(int sock, char *cmd)
{
    char    data[MAX_DATASIZE + 1];
	int rv;

	ft_memset(data, 0, MAX_DATASIZE + 1);
    // Send command
    if ((rv = ftp_send_data(sock, cmd, ft_strlen(cmd))) < 1)
		return (rv);
	// Read and print out response
	if ((rv = ftp_recv_data(sock, &data)) < 1)
		return (rv);
	if (ft_strcmp(UNAUTHORISED_AREA, data) == 0)
		rv = -1;
	ft_putendl(data);
	return (rv);
}

int     ftp_get(int sock, char *cmd)
{
	int	rv;
	
    ftp_error(ERR_INFO, "Get file from server");
    // Send command
    rv = ftp_send_data(sock, cmd, ft_strlen(cmd));
	return (rv);
}

int     ftp_put(int sock, char *cmd)
{
	int	rv;

    ftp_error(ERR_INFO, "Put file onto server");
    // Send command
    rv = ftp_send_data(sock, cmd, ft_strlen(cmd));

// CLIENT
	// Validate local file
	// Setup pipe fds
	// Create child process
	// -- CHILD --
	//		Close pipe[0]
	//		dup2(pipe[1], STDOUT_FILENO)
	//		run xxd on filepath
	// -- PARENT --
	//		close pipe[1]
	//		while ((rv = read(pipe[0]), buf, MAX_DATASIZE) > 0)
	//			ftp_send_data(serverSock, buf, rv)

	return (rv);
}

int     ftp_pwd(int sock, char *cmd)
{
    char    data[MAX_DATASIZE + 1];
	int rv;

	ft_memset(data, 0, MAX_DATASIZE + 1);
    // Send command
    if ((rv = ftp_send_data(sock, cmd, ft_strlen(cmd))) < 1)
		return (rv);
	// Read and print out response
	if ((rv = ftp_recv_data(sock, &data)) < 1)
		return (rv);
	ft_putendl(data);
	return (1);
}

int     ftp_quit(int sock)
{
    ftp_error(ERR_INFO, "Quitting");
    ftp_send_data(sock, "quit", 4);
    return (-1);
}