/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 09:59:43 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/20 13:05:31 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

char	*ftp_remove_filename(char *path)
{
	char	*tmp;
	int		len;

	len = ft_strlen(path);
	tmp = ft_strrchr(path, '/');
	len = len - ft_strlen(tmp);
	while (path[++len])
		path[len] = 0;
	return (path);
}

/*
** Checks if path is lower than root directory
*/

char	*ftp_validate_path(char *root, char *path)
{
	char	buf[4096];
	char	*ret;

	ret = NULL;
	ft_memset(buf, 0, 4096);
	realpath(path, buf);
	if (ft_strncmp(buf, root, ft_strlen(root) - 1) == 0)
		ret = buf;
	return (ret);
}
