/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/11 19:19:32 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/14 13:12:24 by kbamping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_errors.h"

int     ftp_error(enum e_error_code errCode, char *message)
{
    if (errCode == ERR_FATAL)
        ftp_error_fatal(message);
    else if (errCode == ERR_WARN)
        ftp_error_warning(message);
    else if (errCode == ERR_INFO)
        ftp_error_info(message);
    return (errCode);
}

void    ftp_error_fatal(char *message)
{
    ft_fprintf(STDERR_FILENO, "%s[fatal error]%s:%s ", C_RED, C_BOLD, C_NONE);
    ft_putendl_fd(message, STDERR_FILENO);
    exit(1);
}

void    ftp_error_warning(char *message)
{
    ft_fprintf(STDERR_FILENO, "%s[warning]%s::%s ", C_MAGENTA, C_BOLD, C_NONE);
    ft_putendl_fd(message, STDERR_FILENO);
}

void    ftp_error_info(char *message)
{
    ft_fprintf(STDOUT_FILENO, "%s[info]%s::%s ", C_GRAY, C_BOLD, C_NONE);
    ft_putendl_fd(message, STDOUT_FILENO);
}