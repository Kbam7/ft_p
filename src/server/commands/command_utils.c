/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 17:53:34 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/18 15:22:00 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

void	ftp_get_cwd(char **cwd)
{
	char	*tmp;

	if ((*cwd = getcwd(NULL, 0)) != NULL) {
		tmp = ft_strjoin(*cwd, "/");
		ft_memdel((void **)cwd);
		*cwd = tmp;
	}
}

char	*ftp_get_path(t_server *s, char *args)
{
	char	*tmp;

	// root
 	if (ft_strcmp(args, "/") == 0)
		tmp = ft_strdup(s->i.root_path);
	else if (args[0] == '/') // absolute
		tmp = ft_strjoin(s->i.root_path, args + 1);
	else {// relative to pwd
		tmp = ft_strjoinstr(s->i.root_path, s->i.pwd + 1, args);
	}
	return (tmp);
}