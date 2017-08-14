/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/13 18:25:52 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/14 15:29:24 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

void    ftp_init_client(int *sock, char *addr, char *port)
{
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Get info for ip/hostname
    if (getaddrinfo(addr, port, &hints, &servinfo) != 0)
        ftp_error(ERR_FATAL, "addrinfo: Could not get server information");

    // loop through all the results and connect to the first we can
    p = servinfo;
    while (p != NULL) {
        if ((*sock = ftp_connect(p)) == -1) {
            p = p->ai_next;
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo); // all done with this structure
    if (p == NULL || *sock == -1)
        ftp_error(ERR_FATAL, "client: failed to connect\n");
}

int     ftp_connect(struct addrinfo *p)
{
    int             sock;
    struct in_addr  *addr;
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
        addr = (struct in_addr *)get_in_addr(p->ai_addr);
        tmp = ft_strjoin("connected to ", inet_ntoa(*addr));
        ftp_error(ERR_INFO, tmp);
        free(tmp);
        return (sock);
    }
    return (-1);
}