/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_lcd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/17 08:13:58 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/20 12:00:31 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int		ftp_lcd(char *cmd)
{
	if (ftp_is_dir(cmd + 4))
	{
		if (chdir(cmd + 4) != 0)
			ftp_error(ERR_WARN, "Unable to change directory");
		ftp_error(ERR_INFO, "Directory changed!");
	}
	else
		ftp_error(ERR_WARN, "Path is not a directory");
	return (1);
}
