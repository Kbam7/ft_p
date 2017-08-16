/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 17:53:34 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/16 10:13:41 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

void	ftp_get_cwd(char **cwd)
{
	char	*tmp;
	
	// Get cwd, add '/' to end
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
	else // relative to pwd
		tmp = ft_strjoinstr(s->i.root_path, s->i.pwd, args);
	return (tmp);
}