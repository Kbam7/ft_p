/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_network.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 19:01:49 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/14 12:09:19 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_NETWORK_H
# define FTP_NETWORK_H

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

# include "libft.h"
# include "ftp_errors.h"

#define PORT        	"54000"     // the port users will be connecting to
#define BACKLOG     	10          // how many pending connections queue will hold
#define MAX_CLIENTS 	1000        // Max number of clients allowed on server
#define MAX_MSGSIZE		4096

// get sockaddr, IPv4 or IPv6:
void	*get_in_addr(struct sockaddr *sa);
int		ftp_send_msg(int sock, char *buf, int len);
int		ftp_receive_msg(int sock, char (*buf)[]);

#endif /* FTP_NETWORK_H */