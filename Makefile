# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbam7 <kbam7@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/07/17 12:35:16 by kbamping          #+#    #+#              #
#    Updated: 2017/08/17 08:57:58 by kbam7            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#-- PROGRAM | SOURCE FILES --#
SERVER		= server
CLIENT		= client
SRC_PATH	= ./src
SVR_PATH	= $(SRC_PATH)/server
CLT_PATH	= $(SRC_PATH)/client
NTWRK_PATH	= $(SRC_PATH)/network

GLOBAL_SRCS	=	$(NTWRK_PATH)/ftp_errors.c $(NTWRK_PATH)/ftp_network.c		\
				$(NTWRK_PATH)/ftp_filecheck.c								

SVR_SRCS	=	$(SVR_PATH)/server.c $(SVR_PATH)/server_init.c				\
				$(SVR_PATH)/server_signals.c $(SVR_PATH)/connect.c			\
				$(SVR_PATH)/handle_client.c									\
				$(SVR_PATH)/commands/command_utils.c						\
				$(SVR_PATH)/commands/ftp_cd.c								\
				$(SVR_PATH)/commands/ftp_get.c								\
				$(SVR_PATH)/commands/ftp_ls.c								\
				$(SVR_PATH)/commands/ftp_put.c								\
				$(SVR_PATH)/commands/ftp_pwd.c								\
				$(SVR_PATH)/commands/ftp_quit.c								\
				$(SVR_PATH)/commands/validate_path.c						\
				$(GLOBAL_SRCS)

CLT_SRCS	= 	$(CLT_PATH)/client.c $(CLT_PATH)/client_init.c				\
				$(CLT_PATH)/ftp_client_ui.c									\
				$(CLT_PATH)/commands/command_handlers.c						\
				$(CLT_PATH)/commands/ftp_cd.c								\
				$(CLT_PATH)/commands/ftp_get.c								\
				$(CLT_PATH)/commands/ftp_ls.c								\
				$(CLT_PATH)/commands/ftp_put.c								\
				$(CLT_PATH)/commands/ftp_pwd.c								\
				$(CLT_PATH)/commands/ftp_quit.c								\
				$(GLOBAL_SRCS)

SVR_OBJS	= $(SVR_SRCS:%.c=%.o)
CLT_OBJS	= $(CLT_SRCS:%.c=%.o)

#-- COMPILER | LIBS | FLAGS --#
LIBFT 		= -L libft -lft
INCLUDES	= -I include -I libft/includes
WFLAGS		= -Wall -Wextra -Werror -g3
CC			= gcc
CCFLAGS		= $(WFLAGS) $(INCLUDES)

.PHONY: all clean fclean re \
		clean_svr clean_clt \
		fclean_svr fclean_clt \
		run run_valgrind re_libft \
		libft clean_libft fclean_libft


#-- RULES --#
all: libft $(SERVER) $(CLIENT)

# sources
$(SRC_PATH)/%.o : $(SRC_PATH)/%.c
	@$(CC) $(CCFLAGS) -c $< -o $@
	@echo "\033[01;36m[FTP] \t\t\t-- \033[0m$^ \033[01;36m>>\033[00;32m $@ \033[0m"

# server
$(SVR_PATH)/%.o : $(SVR_PATH)/%.c
	@$(CC) $(CCFLAGS) -c $< -o $@
	@echo "\033[01;36m[$(SERVER)] \t\t-- \033[0m$^ \033[01;36m>>\033[00;32m $@ \033[0m"

$(SERVER): $(SVR_OBJS)
	@$(CC) $(CCFLAGS) $^ $(LIBFT) -o $@
	@echo "\033[01;36m[$@] \t\t-- \033[00;32mCOMPILED\033[0m\n"

# client
$(CLT_PATH)/%.o : $(CLT_PATH)/%.c
	@$(CC) $(CCFLAGS) -c $< -o $@
	@echo "\033[01;36m[$(CLIENT)] \t\t-- \033[0m$^ \033[01;36m>>\033[00;32m $@ \033[0m"

$(CLIENT): $(CLT_OBJS)
	@$(CC) $(CCFLAGS) $^ $(LIBFT) -o $@
	@echo "\033[01;36m[$@] \t\t-- \033[00;32mCOMPILED\033[0m\n"

# libft
libft:
	@$(MAKE) -s -C libft/ all

clean_libft:
	@$(MAKE) -s -C libft/ clean

fclean_libft:
	@$(MAKE) -s -C libft/ fclean

re_libft:
	@$(MAKE) -s -C libft/ re

#-- General rules
# clean
clean:
	@rm -f $(SVR_OBJS) $(CLT_OBJS)
	@echo "\033[01;36m[$(SERVER)][$(CLIENT)] \t-- \033[00;32mREMOVED OBJECT FILES\033[0m"

clean_svr:
	@rm -f $(SVR_OBJS)
	@echo "\033[01;36m[$(SERVER)] \t\t-- \033[00;32mREMOVED OBJECT FILES\033[0m"

clean_clt:
	@rm -f $(CLT_OBJS)
	@echo "\033[01;36m[$(CLIENT)] \t\t-- \033[00;32mREMOVED OBJECT FILES\033[0m"

# full clean
fclean: clean
	@rm -f $(SERVER) $(CLIENT)
	@echo "\033[01;36m[$(SERVER)][$(CLIENT)] \t-- \033[00;32mREMOVED EXECUTABLE\033[0m\n"

fclean_svr: clean_svr
	@rm -f $(SERVER)
	@echo "\033[01;36m[$(SERVER)] \t\t-- \033[00;32mREMOVED EXECUTABLE\033[0m\n"

fclean_clt: clean_clt
	@rm -f $(CLIENT)
	@echo "\033[01;36m[$(CLIENT)] \t\t-- \033[00;32mREMOVED EXECUTABLE\033[0m\n"

# run
re: fclean all