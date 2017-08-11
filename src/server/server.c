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
     struct sockaddr_in  server, client; 
    int                 listenSock, newClient;
    char                buf[1024], command[1024];
    unsigned int        len;
    int                 i;

//  For use with getaddrinfo()
/*     memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP


    if (getaddrinfo(NULL, argv[1], &hints, &res))
    {
        dprintf(2, "Cannot get address information for : %s", argv[1]);
        exit(1);
    }
*/
    // Create listen-socket
    if((listenSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        dprintf(2, "Socket creation failed");
        exit(1);
    }

    // Setup listen-socket address and port
    if (argc == 2)
        server.sin_port = atoi(argv[1]);
    else
        server.sin_port = 54000;
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind socket with address
    if(bind(listenSock, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        printf("Binding error");
        close(listenSock);
        exit(1);
    }

    // Listen to socket
    if(listen(listenSock, 10) == -1)
    {
        printf("Listen failed");
        close(listenSock);
        exit(1);

    }

    printf("Waiting for connections..\n");

    // Accept socket requests
    len = sizeof(client);
    newClient = accept(listenSock,(struct sockaddr*)&client, &len);

    struct sockaddr details;
    unsigned int detLen = sizeof(details);
    getsockname(newClient, &details, &detLen);
    printf("Accepted connection..\n");
    while(1)
    {
        // Read from new client
        recv(newClient, buf, 1024, 0);

        // Get a single command from the buf
        sscanf(buf, "%s", command);
        
        if (!strcmp(command, "bye") || !strcmp(command, "quit"))
        {
            printf("FTP server quitting..\n");
            i = 1;
            send(newClient, &i, sizeof(int), 0);
            close(listenSock);
            close(newClient);
            exit(0);
        }

        // Echo back received data
        send(newClient, buf, 1024, 0);
    }
    return 0;
}