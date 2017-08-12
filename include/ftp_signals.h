/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_signals.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 14:54:19 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/12 14:58:10 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_SIGNALS_H
# define FTP_SIGNALS_H

# include <errno.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

void	sigchld_handler(int s);

#endif /* FTP_SIGNALS_H */