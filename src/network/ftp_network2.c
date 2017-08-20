/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_network2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/16 10:49:33 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/20 12:27:02 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_network.h"

void	ftp_extract_data(int fd, char (*data)[])
{
	int		offset;
	int		rem;
	int		keylen;
	char	*tmp;

	rem = ft_strlen(ft_strstr(*data, FTP_DATA_END_KEY));
	keylen = ft_strlen(FTP_DATA_END_KEY);
	offset = ft_strlen(*data) - rem;
	if (offset > 0)
	{
		tmp = ft_strsub(*data, 0, offset);
		write(fd, tmp, ft_strlen(tmp));
		ft_memdel((void **)&tmp);
	}
	tmp = ft_strsub(*data, offset + keylen, rem - keylen);
	write(fd, tmp, ft_strlen(tmp));
	ft_memdel((void **)&tmp);
}

int		ftp_write_from_socket(int sock, int fd)
{
	char	data[MAX_DATASIZE + 1];
	int		rv;

	ft_memset(data, 0, MAX_DATASIZE + 1);
	while (1)
	{
		if ((rv = ftp_recv_data(sock, &data)) < 1)
			break ;
		if (ft_strstr(data, FTP_DATA_END_KEY))
		{
			if (ft_strcmp(data, FTP_DATA_END_KEY) != 0)
				ftp_extract_data(fd, &data);
			break ;
		}
		write(fd, data, rv);
		ft_memset(data, 0, rv + 1);
	}
	return (rv);
}

int		ftp_read_fd_write_sock(int fd, int sock)
{
	int		rv;
	char	data[MAX_DATASIZE + 1];

	rv = 0;
	ft_memset(data, 0, MAX_DATASIZE + 1);
	while ((rv = read(fd, data, MAX_DATASIZE)) > 0)
	{
		if ((rv = ftp_send_data(sock, data, rv)) < 1)
			break ;
		ft_memset(data, 0, MAX_DATASIZE + 1);
	}
	ftp_send_data(sock, FTP_DATA_END_KEY, ft_strlen(FTP_DATA_END_KEY));
	if (rv < 0)
		return (rv);
	return (1);
}
