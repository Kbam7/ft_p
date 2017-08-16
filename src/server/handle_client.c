/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 18:28:16 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/16 11:16:37 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

int		ftp_handle_client_input(t_server *s, int sock);
int		ftp_parse_input(t_server *s, int sock, char *buf);

/* Handles new client */
void    ftp_handle_client(t_server *server, t_client *client)
{
	int	rv;

    if ((client->pid = fork()) == -1) {
        ftp_error(ERR_WARN, "server: Could not fork for new connection");
        ftp_disconnect_client(server, client->index);
    }
    else if (client->pid == 0) {
        close(server->listenSocket);
        while (1) {
			ftp_error(ERR_INFO, "Waiting for input...");
			rv = ftp_handle_client_input(server, client->socket);
			if (rv == 0 || rv == -1) // Disconnected / Error
				break;
        }

        // End of client loop -  Disconnect client
        ftp_disconnect_client(server, client->index);
        exit(0);
    }
}

int     ftp_handle_client_input(t_server *s, int sock)
{
    int     rv;
    char    buf[MAX_MSGSIZE + 1];
    
    
    // Receive data
    ft_memset(buf, 0, MAX_MSGSIZE + 1);
    if ((rv = ftp_recv_data(sock, &buf)) < 1)
        return (rv);

    // Print out data
    ft_printf("Server received %d bytes: %s\n", rv, buf);

    // Parse input
    return (ftp_parse_input(s, sock, buf));
}

int     ftp_parse_input(t_server *s, int sock, char *buf)
{
    t_split_string	sp;
    char			*args;
    int				rv;
    
    sp = ft_nstrsplit(buf, ' ');
    if (sp.words < 2)
        args = NULL;
    else
        args = sp.strings[1];
    if (ft_strcmp(sp.strings[0], "ls") == 0)
        rv = ftp_ls(s, sock, args);
    else if (ft_strcmp(sp.strings[0], "cd") == 0)
        rv = ftp_cd(s, sock, args);
    else if (ft_strcmp(sp.strings[0], "get") == 0)
        rv = ftp_get(s, sock, args);
    else if (ft_strcmp(sp.strings[0], "put") == 0)
        rv = ftp_put(s, sock, args);
    else if (ft_strcmp(sp.strings[0], "pwd") == 0)
        rv = ftp_pwd(s, sock, args);
    else if (ft_strcmp(sp.strings[0], "quit") == 0)
        rv = ftp_quit();
    else
		rv = ftp_error(ERR_WARN, "Unrecognised command");
    ft_free_split(&sp);
    return(rv);
}