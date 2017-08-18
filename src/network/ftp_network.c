/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_network.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/16 10:49:33 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/18 14:33:10 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_network.h"

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int	check_send_rv(int rv)
{
	if (rv == -1)
		ftp_error(ERR_WARN, "send: Failed to send message");
	if (rv == 0)
		ftp_error(ERR_WARN, "send: zero bytes sent"); // ?
	return (rv);
}

int	check_recv_rv(int rv)
{
	if (rv == -1)
		ftp_error(ERR_WARN, "read: Failed to read data");
	if (rv == 0)
		ftp_error(ERR_WARN, "read: zero bytes read"); // ?
	return (rv);
}

int ftp_send_data(int sock, char *data, int len)
{
    int     rv;
	char	buf[2];

ftp_error(ERR_WARN, "send data - start");	// debug

	// Send length of data
	if ((rv = send(sock, ft_itoa(len), UNIVERSAL_ANSWER, 0)) < 1)
		return (check_send_rv(rv));

ftp_error(ERR_WARN, "send data - waiting for confirmation");	// debug

	// Wait for confirmation
	ft_memset(buf, 0, 2);
	if ((rv = recv(sock, buf, 1, 0)) < 1)
		return (check_send_rv(rv));

ftp_error(ERR_WARN, "send data - confirmation received");	// debug

	// Validate confirmation
	if (ft_strcmp(buf, "1") == 0) {

ftp_error(ERR_WARN, "send data - Valid confirmtion, sending data");	// debug

		if ((rv = send(sock, data, len, 0)) < 1)
			return (check_send_rv(rv));
	} else
		return (-1);

ftp_error(ERR_WARN, "send data - end");	// debug

    return (rv);
}

int ftp_recv_data(int sock, char (*data)[])
{
    int     rv;
	int		len;
	char	buf[UNIVERSAL_ANSWER + 1];
    
ftp_error(ERR_WARN, "recv data - start");	// debug

	ft_memset(buf, 0, UNIVERSAL_ANSWER + 1);
	// Read MAX_MSGSIZE bytes, expecting length of data
    if ((rv = recv(sock, buf, UNIVERSAL_ANSWER, 0)) < 1)
        return (check_recv_rv(rv));
	if (ft_isint(buf) && (len = ft_atoi(buf)) > 0 && len <= MAX_DATASIZE) {
		// Send confirmation
		if ((rv = send(sock, "1", 1, 0)) < 1)
			return (check_recv_rv(rv));

ftp_error(ERR_WARN, "recv data - confirmation sent, reading data");	// debug

		// Read length bytes from socket
		if ((rv = recv(sock, *data, len, 0)) < 1)
        	return (check_recv_rv(rv));
	} else {
		// Invalid datasize
		if ((rv = send(sock, "0", 1, 0)) < 1)
			return (check_recv_rv(rv));
		return (-1);// Invalid datasize
	}
ftp_error(ERR_WARN, "recv data - end");	// debug

    return (rv);
}