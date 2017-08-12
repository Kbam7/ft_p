/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_errors.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/11 19:28:30 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/11 21:43:53 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_ERRORS_H
# define FTP_ERRORS_H

# include <stdlib.h>
# include "libft.h"

enum e_error_code {
    ERR_NONE = 0,
    ERR_FATAL,
    ERR_WARN,
    ERR_INFO
};

// Error handler
void    ftp_error(enum e_error_code errCode, char *message);

// Error functions
void    ftp_error_fatal(char *message);
void    ftp_error_warning(char *message);
void    ftp_error_info(char *message);

#endif /* FTP_ERRORS_H */