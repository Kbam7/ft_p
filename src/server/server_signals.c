/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 12:33:46 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/12 14:55:05 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_signals.h"

// Clear zombie processes
void sigchld_handler(int s)
{
    (void)s;
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}