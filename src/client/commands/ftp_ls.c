/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_ls.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/17 08:13:58 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/20 12:11:21 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int		ftp_ls(int sock, char *cmd)
{
	char	data[MAX_DATASIZE + 1];
	int		rv;

	ft_memset(data, 0, MAX_DATASIZE + 1);
	if ((rv = ftp_send_data(sock, cmd, ft_strlen(cmd))) < 1)
		return (rv);
	if ((rv = ftp_write_from_socket(sock, STDOUT_FILENO)) < 1)
		ftp_error(ERR_INFO, "command failed");
	return (rv);
}
