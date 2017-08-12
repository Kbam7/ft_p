/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 12:29:32 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/12 19:11:58 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"

/*
    Accepts client connections and adds them to client array.
    Returns address to new client
*/
t_client	*ftp_accept_client(t_server *server)
{
    struct sockaddr_storage	clt_addr;
    t_client				*clt;
	char					*msg;

	clt = (t_client *)ft_memalloc(sizeof(t_client));
	server->client_array[server->n_clients] = clt;
	clt->index = server->n_clients;
    clt->size = sizeof(clt_addr);
    clt->socket = accept(server->listenSocket,
					(struct sockaddr *)&clt_addr, &(clt->size));
    if (clt->socket == -1) {
        ftp_error(ERR_WARN, "accept: Unable to accept client");
		return (NULL);
	}
	clt->address = (struct in_addr *)get_in_addr((struct sockaddr *)&clt_addr);
	msg = ft_strjoin("server: got connection from ", inet_ntoa(*(clt->address)));
	ftp_error(ERR_INFO, msg);
	free(msg);
	server->n_clients++;
	return (clt);
}

/* Disconnects client from server */
void	ftp_disconnect_client(t_server *server, unsigned int i_client)
{
	t_client	*client;
	char		*msg;

	// Check if valid client number
    if (server->n_clients >= i_client + 1) {
		client = server->client_array[i_client];
		msg = "server: Disconnected client ";
		msg = ft_strjoin(msg, inet_ntoa(*(client->address)));
		close(client->socket);
		ft_memset(client, 0, sizeof(client));
		server->n_clients--;
		ftp_error(ERR_INFO, msg);
		free(msg);
	}
}