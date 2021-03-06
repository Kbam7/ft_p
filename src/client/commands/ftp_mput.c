/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_mput.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/17 08:13:58 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/20 13:28:10 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int		ftp_mput(int sock, char *cmd)
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
			tmp = ft_strjoin("put ", sp.strings[i]);
			if (ftp_put(sock, tmp) < 1)
				rv = 0;
			ft_memdel((void **)&tmp);
			sleep(1);
		}
	}
	ft_free_split(&sp);
	return (rv);
}
