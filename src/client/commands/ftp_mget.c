/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_mget.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/17 08:13:58 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/19 18:02:21 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int     ftp_mget(int sock, char *cmd)
{
	t_split_string	sp;
	size_t			i;
	int				rv;
	char			*tmp;

	rv = 1;
	sp = ft_nstrsplit(cmd, ' ');
	if (sp.words > 1)
	{
		i = 0;
		while (rv && ++i < sp.words)
		{
			tmp = ft_strjoin("get ", sp.strings[i]);
			if (ftp_get(sock, tmp) < 1)
				rv = 0;
			ft_memdel((void **)&tmp);
		}
	}
	ft_free_split(&sp);
	return (rv);
}

/*

	int		rv;
	char	data[MAX_DATASIZE + 1];

	rv = 1;
	if (!ftp_file_exists(cmd + 4) || ftp_validate_overwrite())
	{
		if ((rv = ftp_send_data(sock, cmd, ft_strlen(cmd))) > 0)
		{
			ft_memset(data, 0, MAX_DATASIZE + 1);
			if ((rv = ftp_recv_data(sock, &data)) > 0)
			{
				if (ft_strcmp(data, "writing") == 0)
					if ((rv = ftp_get_write(sock, cmd + 4)) > 0)
						ftp_error(ERR_INFO, "File received");
				if (ft_strncmp(data, "failed: ", 8) == 0)
					ftp_error(ERR_WARN, data);
			}
		}
	}
	return (rv);

*/