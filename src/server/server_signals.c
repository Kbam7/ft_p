/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 12:33:46 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/20 12:43:13 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_signals.h"

/*
** Clear zombie processes
*/

void	sigchld_handler(int s)
{
	int	saved_errno;

	(void)s;
	saved_errno = errno;
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
	errno = saved_errno;
}
