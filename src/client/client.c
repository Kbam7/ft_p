/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 19:38:23 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/12 20:43:11 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "ftp_network.h"
#include "ftp_errors.h"

void    ftp_init_client(int *sock, char *addr, char *port);
int     ftp_connect(struct addrinfo *p);
void    ftp_print_client_menu(void);

int main(int argc, char *argv[])
{
    int sockfd;
    int n;  
    char buf[MAX_READSIZE];
    int quit = 0;

    if (argc < 2)
        ftp_error(ERR_FATAL, "usage: client hostname port\n");
    if (argc < 3) {
        ftp_error(ERR_WARN,"Using default port ("PORT")\n");
        ftp_init_client(&sockfd, argv[1], PORT);
    } else
        ftp_init_client(&sockfd, argv[1], argv[2]);

    while (!quit)
    {
        ftp_print_client_menu();
        
        // Get input from keyboard
        fgets(buf, MAX_READSIZE, stdin);

        // Send user input to server
        send(sockfd, buf, ft_strlen(buf), 0);

        // Read response
        if ((n = recv(sockfd, buf, MAX_READSIZE - 1, 0)) == -1) {
            ftp_error(ERR_WARN, "recv: Failed to receive data");
        }

        // If there is a response, print out response
        if (n > 0)
        {
            buf[n] = '\0';
            printf("client: received '%s'\n",buf);
            if (strcmp(buf, "quit") == 0)
                break;
        }
    }
    close(sockfd);

    return 0;
}

void    ftp_print_client_menu(void)
{
    char    *menu;

    menu = "\t---[FTP SERVER]---\n\n\t\t-[MENU]-\n"
    " # 1\t- ls\n # 2\t- cd\n # 3\t- get\n # 4\t- put\n # 5\t- pwd\n # 6\t- quit\n";
    ft_fprintf(STDOUT_FILENO, menu);    
}

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
    if (p == NULL)
        ftp_error(ERR_FATAL, "client: failed to connect\n");
}

int     ftp_connect(struct addrinfo *p)
{
    int             sock;
    struct in_addr  *addr;
    char            *tmp;

    sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
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