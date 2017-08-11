# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbamping <kbamping@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/07/17 12:35:16 by kbamping          #+#    #+#              #
#    Updated: 2017/08/11 14:11:46 by kbamping         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#-- PROGRAM | SOURCE FILES --#
SERVER		= server
CLIENT		= client
SVR_PATH	= ./src/server
CLT_PATH	= ./src/client


SVR_SRCS	= $(SVR_PATH)/server2.c
CLT_SRCS	= $(CLT_PATH)/client2.c

SVR_OBJS	= $(SVR_SRCS:%.c=%.o)
CLT_OBJS	= $(CLT_SRCS:%.c=%.o)

#-- COMPILER | LIBS | FLAGS --#
INCLUDES	= -I ./include
WFLAGS		= -Wall -Wextra -Werror -g3
CC			= gcc
CCFLAGS		= $(WFLAGS) $(INCLUDES)

.PHONY: all client server clean fclean re \
		clean_svr clean_clt \
		fclean_svr fclean_clt \
		run run_valgrind


#-- RULES --#
all: $(SERVER) $(CLIENT)

# server
$(SVR_PATH)/%.o : $(SVR_PATH)/%.c
	@$(CC) $(CCFLAGS) -c $< -o $@
	@echo "\033[01;36m[$(SERVER)] \t\t-- \033[0m$^ \033[01;36m>>\033[00;32m $@ \033[0m"

$(SERVER): $(SVR_OBJS)
	@$(CC) $(CCFLAGS) $^ -o $@
	@echo "\033[01;36m[$@] \t\t-- \033[00;32mCOMPILED\033[0m\n"

# client
$(CLT_PATH)/%.o : $(CLT_PATH)/%.c
	@$(CC) $(CCFLAGS) -c $< -o $@
	@echo "\033[01;36m[$(CLIENT)] \t\t-- \033[0m$^ \033[01;36m>>\033[00;32m $@ \033[0m"

$(CLIENT): $(CLT_OBJS)
	@$(CC) $(CCFLAGS) $^ -o $@
	@echo "\033[01;36m[$@] \t\t-- \033[00;32mCOMPILED\033[0m\n"

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

## NEED TO MAKE A PROGRAM THAT SIMULATES A   CLT -> SVR  INTERACTION
#run:
#	@rm -f $(PROG)
#	@echo "\033[01;36m$(PROG) \t\t-- \033[00;32mREMOVED EXECUTABLE\033[0m\n"
#	@$(MAKE) -s all
#	./$(PROG) example.avm

#run_valgrind:
#	valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./$(PROG) example.avm