/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 19:38:23 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/15 18:22:24 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int main(int argc, char *argv[])
{
    int sock;

    if (argc < 2 || argc > 3)
        ftp_error(ERR_FATAL, "usage: client hostname port\n");
    if (argc < 3) {
        ftp_error(ERR_WARN,"Using default port ("PORT")\n");
        ftp_init_client(&sock, argv[1], PORT);
    } else
        ftp_init_client(&sock, argv[1], argv[2]);

    while (1) {
        ftp_print_prompt();
        if (ftp_handle_user_commands(sock) == -1)
			break;
    }
    ftp_error(ERR_INFO, "Closing client...");
    close(sock);

    return (EXIT_SUCCESS);
}