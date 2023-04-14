// client.c -- TCP Socket Client with file fetching capability

#include "client_utils.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int socket_desc;
    struct sockaddr_in server_addr;

    if (argc < 3)
    {
        printf("Usage: fget GET remote-file-path local-file-path\n");
        printf("Usage: fget INFO remote-file-path\n");
        printf("Usage: fget MD remote-folder-path\n");
        printf("Usage: fget PUT local-file-path remote-file-path\n");
        printf("Usage: fget RM remote-file-path\n");
        return -1;
    }

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0)
    {
        perror("Unable to create socket");
        return -1;
    }

    printf("Socket created successfully\n");

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("10.211.55.6");

    // Send connection request to server:
    if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Unable to connect");
        return -1;
    }
    printf("Connected with server successfully\n");

    int result = process_command(argc, argv, socket_desc);

    close(socket_desc);

    return result;
}
