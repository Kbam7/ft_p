/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 19:38:23 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/17 10:16:42 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int main(int argc, char *argv[])
{
    int			sock;
	t_session	sess;

    if (argc < 2 || argc > 3)
        ftp_error(ERR_FATAL, "usage: client hostname port\n");
    if (argc < 3) {
        ftp_error(ERR_WARN,"Using default port ("PORT")\n");
        sess.port = PORT;
    } else
        sess.port = argv[2];
	ftp_init_client(&sess, &sock, argv[1]);
	sess.socket = sock;
    while (1) {
        ftp_print_prompt();
        if (ftp_handle_user_commands(&sess) == -1)
			break;
    }
    ftp_error(ERR_INFO, "Closing client...");
    close(sock);

    return (EXIT_SUCCESS);
}