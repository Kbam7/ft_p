/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/13 18:25:52 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/17 08:52:36 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int     ftp_connect(t_session *s, struct addrinfo *p)
{
    int             sock;
    char            *tmp;

    sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
	//sock = (fcntl(sock, O_NONBLOCK) == 0) ? sock : -1;
    if (sock == -1)
        ftp_error(ERR_WARN, "client: Unable to create socket");
    else if (connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
        close(sock);
        ftp_error(ERR_WARN, "client: Could not connect to server");
    }
    else {
        s->address = (struct in_addr *)get_in_addr(p->ai_addr);
        tmp = ft_strjoin("connected to ", inet_ntoa(*s->address));
        ftp_error(ERR_INFO, tmp);
        free(tmp);
        return (sock);
    }
    return (-1);
}

void    ftp_init_client(t_session *s, int *sock, char *addr)
{
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(addr, s->port, &hints, &servinfo) != 0)
        ftp_error(ERR_FATAL, "addrinfo: Could not get server information");
    p = servinfo;
    while (p != NULL) {
        if ((*sock = ftp_connect(s, p)) == -1) {
            p = p->ai_next;
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo);
    if (p == NULL || *sock == -1)
        ftp_error(ERR_FATAL, "client: failed to connect\n");
	s->root_path = getcwd(NULL, 0);
}