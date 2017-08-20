/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_network.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/16 10:49:33 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/20 12:25:32 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_network.h"

void	*get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return (&(((struct sockaddr_in*)sa)->sin_addr));
	return (&(((struct sockaddr_in6*)sa)->sin6_addr));
}

int		check_send_rv(int rv)
{
	if (rv == -1)
		ftp_error(ERR_WARN, "send: Failed to send message");
	if (rv == 0)
		ftp_error(ERR_WARN, "send: zero bytes sent");
	return (rv);
}

int		check_recv_rv(int rv)
{
	if (rv == -1)
		ftp_error(ERR_WARN, "read: Failed to read data");
	if (rv == 0)
		ftp_error(ERR_WARN, "read: zero bytes read");
	return (rv);
}

int		ftp_send_data(int sock, char *data, int len)
{
	int	rv;

	if ((rv = send(sock, data, len, 0)) < 1)
		return (check_send_rv(rv));
	return (rv);
}

int		ftp_recv_data(int sock, char (*data)[])
{
	int	rv;

	if ((rv = recv(sock, *data, MAX_DATASIZE, 0)) < 1)
		return (check_recv_rv(rv));
	return (rv);
}
