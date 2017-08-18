/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/17 08:13:58 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/17 08:15:36 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

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