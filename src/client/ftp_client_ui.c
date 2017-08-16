/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_ui.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/15 18:07:08 by kbam7             #+#    #+#             */
/*   Updated: 2017/08/15 18:25:56 by kbam7            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_client.h"

int		ftp_print_prompt(void)
{
	char	*path;

	path = "path";
    ft_printf("[%sFTPCLient%s] %s>", C_GREEN, C_NONE, path);
	return (1);
}

int		ftp_print_client_menu(void)
{
    char    *menu;

    menu = "\t---[FTP SERVER]---\n\n  -[MENU]-\n"
    " #:-  ls\t- List files in current directory\n"
    " #:-  cd\t- Change directory\n"
    " #:-  get\t- Download a file\n"
    " #:-  put\t- Upload a file\n"
    " #:-  pwd\t- View current directory\n"
    " #:-  quit\t- Close the FTP client\n";
    ft_putendl_fd(menu, STDOUT_FILENO);
	return (1);
}