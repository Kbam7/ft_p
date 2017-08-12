/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/11 20:40:20 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/12 20:53:12 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

void    ftp_handle_client_input(int sock);

int main(void)
{
    t_server    server;
    t_client    *client;

    // /server.n_clients = 0;
    init_server(&server);
    while(1) {
        ftp_error(ERR_INFO, "server: waiting for connections...");
        if ((client = ftp_accept_client(&server)) == NULL)
            continue;
        ftp_handle_client(&server, client);
    }
    // Server loop ended
    close(server.listenSocket);
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
            ftp_handle_client_input(client->socket); 
        }

        // End of client loop -  Disconnect client
        ftp_disconnect_client(server, client->index);
        exit(0);
    }
}

void    ftp_handle_client_input(int sock)
{
    int     rv;
    char    buf[MAX_READSIZE];
    char    *buf2;
    
    // Receive data
    rv = recv(sock, &buf, MAX_READSIZE - 1, 0);
    buf[rv] = '\0';
    buf2 = ft_strtrim(buf);
    // Print out data
    ft_printf("Server received %d bytes: %s\n", rv, buf2);
    // Echo back
    send(sock, buf2, ft_strlen(buf2), 0);
    free(buf2);

/*     if (strcmp(buf, "quit") == 0)
    {
        send(sock, "Server Quitting...", 11, 0);
    } */
}