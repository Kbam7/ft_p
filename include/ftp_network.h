/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_network.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/12 19:01:49 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/16 11:08:51 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_NETWORK_H
# define FTP_NETWORK_H

# include <netdb.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>

# include "libft.h"
# include "ftp_errors.h"

# define PORT        	"54000"     // the port users will be connecting to
# define BACKLOG     	10          // how many pending connections queue will hold
# define MAX_CLIENTS 	1000        // Max number of clients allowed on server
# define MAX_MSGSIZE	4096
# define MAX_DATASIZE	(MAX_MSGSIZE * 4 < 16384) ? MAX_MSGSIZE * 4 : 16384

# define UNIVERSAL_ANSWER	42
# define UNAUTHORISED_AREA	"You cannot access that stuff!"

// get sockaddr, IPv4 or IPv6:
void	*get_in_addr(struct sockaddr *sa);
int		ftp_send_data(int sock, char *data, int len);
int		ftp_recv_data(int sock, char (*data)[]);

#endif /* FTP_NETWORK_H */