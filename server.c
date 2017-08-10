/*FTP server*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
/* #include <sys/socket.h>
#include <netinet/in.h> */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res, *tmp;
    /* struct sockaddr_in  server, client; */
    int                 sock1, sock2;
    char                buf[1024], command[1024];
    unsigned int        len;
    int                 i;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP


    if (getaddrinfo(NULL, argv[1], &hints, &res))
    {
        dprintf(2, "Cannot get address information for : %s", argv[1]);
        exit(1);
    }

    // Create server socket
    if((sock1 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        dprintf(2, "Socket creation failed");
        exit(1);
    }

    // Setup socket address
/*     server.sin_port = atoi(argv[1]);
    server.sin_addr.s_addr = INADDR_ANY; */

    // Bind socket with address
    if(bind(sock1, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        printf("Binding error");
        close(sock1);
        exit(1);
    }

    // Listen to socket
    if(listen(sock1, 10) == -1)
    {
        printf("Listen failed");
        close(sock1);
        exit(1);

    }

    // Accept socket requests
    len = sizeof(client);
    sock2 = accept(sock1,(struct sockaddr*)&client, &len);
    while(1)
    {
        // Read from socket
        recv(sock2, buf, 1024, 0);
        sscanf(buf, "%s", command);
        
        if (!strcmp(command, "bye") || !strcmp(command, "quit"))
        {
            printf("FTP server quitting..\n");
            i = 1;
            send(sock2, &i, sizeof(int), 0);
            close(sock1);
            close(sock2);
            exit(0);
        }
    }
    return 0;
}