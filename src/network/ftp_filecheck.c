/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_filecheck.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/17 08:58:32 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/17 10:12:59 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_network.h"

int		ftp_is_reglr_file(const char *path)
{
	struct stat	path_stat;

	lstat(path, &path_stat);
	if ((path_stat.st_mode & S_IFMT) == S_IFREG)
		return (1);
	return (0);
}

int		ftp_is_symlink(const char *path)
{
	struct stat		path_stat;

	lstat(path, &path_stat);
	if ((path_stat.st_mode & S_IFMT) == S_IFLNK)
		return (1);
	return (0);
}

int		ftp_is_dir(const char *path)
{
	struct stat		path_stat;

	lstat(path, &path_stat);
	if ((path_stat.st_mode & S_IFMT) == S_IFDIR)
		return (1);
	return (0);
}

int		ftp_file_exists(char *filename)
{
	struct stat		buffer;

	return ((lstat(filename, &buffer) == 0) ? 1 : 0);
}