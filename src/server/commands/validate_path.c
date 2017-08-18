/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/14 09:59:43 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/17 16:53:42 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

char	*ftp_remove_filename(char *path)
{
	char	*tmp;
	int 	len;

	len = ft_strlen(path);
	tmp = ft_strrchr(path, '/');
	len = len - ft_strlen(tmp);
	while (path[++len])
		path[len] = 0;
	return (path);
}

/* Checks if path is lower than root directory */
char	*ftp_validate_path(char *root, char *path)
{
	char	buf[4096];
	char	*ret;
	
ft_putendl("Validate path - strt");	// debug

	ret = NULL;
	ft_memset(buf, 0, 4096);
	ft_putendl(path);				 // debug
	realpath(path, buf);
	ft_putendl(buf);				 // debug

ft_putendl("Validate path - end");	 // debug

	if (ft_strncmp(buf, root, ft_strlen(root) - 1) == 0) 
		ret = buf;
	return (ret);
}