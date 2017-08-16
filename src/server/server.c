/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/11 20:40:20 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/15 18:33:41 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

void	ftp_shutdown_server(t_server *s)
{
	int i;

	ftp_error(ERR_WARN, "Shutting down server...");
	i = 0;
	close(s->listenSocket);
	while (i < MAX_CLIENTS) {
		if (s->client_array[i] != NULL)
			ftp_disconnect_client(s, i);
	}
	free(s->i.root_path);
	free(s->i.pwd);

}

int     main(int argc, char **argv)
{
    t_server    server;
    t_client    *client;
	int i;

	if (argc < 2)
		ftp_error(ERR_INFO, "Using default port 54000");
	else {
		i = ft_atoi(argv[1]);
		if (i < 0 || i > 65535)
			ftp_error(ERR_FATAL, "Port number invalid, try [0 - 65535]");
	}
	init_server(&server, argc, argv);
	while(1) {
        ftp_error(ERR_INFO, "server: waiting for connections...");
        if ((client = ftp_accept_client(&server)) == NULL)
            continue;
        ftp_handle_client(&server, client);
    }
    // Server loop ended
	ftp_shutdown_server(&server);
	return (EXIT_SUCCESS);
}