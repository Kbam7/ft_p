cd /goinfre/kbamping/Desktop/ft_p/include
norminette ftp_client.h
norminette ftp_errors.h
norminette ftp_network.h
norminette ftp_server.h
norminette ftp_signals.h

cd /goinfre/kbamping/Desktop/ft_p/src
client
network
server

cd /goinfre/kbamping/Desktop/ft_p/src/client
norminette client.c
norminette client_init.c
commands
norminette ftp_client_ui.c

cd /goinfre/kbamping/Desktop/ft_p/src/client/commands
norminette command_handlers.c
norminette ftp_cd.c
norminette ftp_get.c
norminette ftp_lcd.c
norminette ftp_lls.c
norminette ftp_lpwd.c
norminette ftp_ls.c
norminette ftp_mget.c
norminette ftp_mput.c
norminette ftp_put.c
norminette ftp_pwd.c
norminette ftp_quit.c

cd /goinfre/kbamping/Desktop/ft_p/src/network
norminette ftp_errors.c
norminette ftp_filecheck.c
norminette ftp_network.c

cd /goinfre/kbamping/Desktop/ft_p/src/server
commands
norminette connect.c
norminette handle_client.c
norminette server.c
norminette server_init.c
norminette server_signals.c

cd /goinfre/kbamping/Desktop/ft_p/src/server/commands
norminette command_utils.c
norminette ftp_cd.c
norminette ftp_get.c
norminette ftp_ls.c
norminette ftp_put.c
norminette ftp_pwd.c
norminette ftp_quit.c
norminette validate_path.c