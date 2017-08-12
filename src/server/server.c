/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/11 20:40:20 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/12 14:52:41 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

int main(void)
{
    t_server    *server;
    t_client    *client;

    server = NULL;
    init_server(server);
    ftp_error(ERR_INFO, "server: waiting for connections...");
    while(1) {
        if ((client = ftp_accept_client(server)) == NULL)
            continue;
        ftp_handle_client(server, client);
    }
    // Server loop ended
    close(server->listenSocket);
    return (EXIT_SUCCESS);
}

/* Handles new client */
void    ftp_handle_client(t_server *server, t_client *client)
{
    // Create new child process for client
    if ((client->pid = fork() == -1)) {
        ftp_error(ERR_WARN, "server: Could not fork for new connection");
        ftp_disconnect_client(server, client->index);
    }
    else if (client->pid == 0) {
        // child doesn't need the listener
        close(server->listenSocket);
        // Init client
        
        // Client loop
        while (1)
            ftp_handle_client_input(client->socket); 

        // End of client loop -  Disconnect client
        ftp_disconnect_client(server, client->index);
        exit(0);
    }
}

void    ftp_handle_client_input(int sock)
{
    int     rv;
    char    buf[MAX_READSIZE];
    // Receive data
    rv = recv(sock, &buf, 1024, 0);

    buf[rv] = '\0';
    // Print out data
    printf("Server received: %s\n", buf);

    // Get server input
    fgets(buf, 1024, stdin);
    // Echo back
    send(sock, buf, rv, 0);

    if (strcmp(buf, "quit") == 0)
    {
        send(sock, "Server Quitting...", 11, 0);
    }
}