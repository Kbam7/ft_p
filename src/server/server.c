/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/11 20:40:20 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/14 15:07:41 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

int     ftp_handle_client_input(t_server *s, int sock);
int     ftp_parse_input(t_server *s, int sock, char *buf);
void	ftp_shutdown_server(t_server *s);

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

/* Handles new client */
void    ftp_handle_client(t_server *server, t_client *client)
{
    // Create new child process for client
    if ((client->pid = fork()) == -1) {
        ftp_error(ERR_WARN, "server: Could not fork for new connection");
        ftp_disconnect_client(server, client->index);
    }
    else if (client->pid == 0) {
        // child doesn't need the listener
        close(server->listenSocket);
        // Init client

        // Client loop
        while (1) {
            //ftp_print_client_menu(client->socket);
			ftp_error(ERR_INFO, "Waiting for input...");
			if (ftp_handle_client_input(server, client->socket) == 0) // Client disconnected
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
    if ((rv = ftp_receive_msg(sock, &buf)) < 1)
        return (0);

    // Print out data
    ft_printf("Server received %d bytes: %s\n", rv, buf);

/*     // Echo back
    if (ftp_send_msg(sock, buf, ft_strlen(buf)) < 1)
        return (0); */

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