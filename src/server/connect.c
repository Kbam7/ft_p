/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 12:29:32 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/14 15:29:19 by kbamping         ###   ########.fr       */
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
	int 					i;

	i = 0;
	clt = (t_client *)ft_memalloc(sizeof(t_client));

	// Insert into first free slot
	while (i < MAX_CLIENTS) {
		if (server->client_array[i] == NULL) {
			server->client_array[i] = clt;
			break;
		}
		++i;
	}
	server->client_array[server->i.n_clients] = clt;
	clt->index = server->i.n_clients;
    clt->size = sizeof(clt_addr);
    clt->socket = accept(server->listenSocket,
					(struct sockaddr *)&clt_addr, &(clt->size));
	//clt->socket = (fcntl(clt->socket, O_NONBLOCK) == 0) ? clt->socket : -1;
	if (clt->socket == -1)
	{
		ftp_error(ERR_WARN, "accept: Unable to accept client");
		return (NULL);
	}
	clt->address = (struct in_addr *)get_in_addr((struct sockaddr *)&clt_addr);
	msg = ft_strjoin("server: got connection from ", inet_ntoa(*(clt->address)));
	ftp_error(ERR_INFO, msg);
	free(msg);
	server->i.n_clients++;
	return (clt);
}

/* Disconnects client from server */
void	ftp_disconnect_client(t_server *server, unsigned int i_client)
{
	t_client	*client;
	char		*msg;

	// Check if valid client number
    if (server->i.n_clients >= i_client + 1) {
		client = server->client_array[i_client];
		msg = "server: Disconnected client ";
		msg = ft_strjoin(msg, inet_ntoa(*(client->address)));
		close(client->socket);
		server->client_array[i_client] = NULL;
		ft_memdel((void **)&client);
		server->i.n_clients--;
		ftp_error(ERR_INFO, msg);
		free(msg);
	}
}