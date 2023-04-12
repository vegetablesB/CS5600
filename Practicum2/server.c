// server.c -- TCP Socket Server with file reading capability

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define BUFFER_SIZE 1024 * 10

void handle_get(int client_sock, char *file_path_end)
{
    int file;
    char client_message[BUFFER_SIZE];
    ssize_t bytes_read;
    char file_path[BUFFER_SIZE - 1024] = "/media/ning/4024-5A83/";
    strcat(file_path, file_path_end);
    printf("File path: %s\n", file_path);
    file = open(file_path, O_RDONLY);
    if (file < 0)
    {
        perror("Error opening file");
        strcpy(client_message, "Error: File not found.");
        send(client_sock, client_message, strlen(client_message), 0);
    }
    else
    {
        while ((bytes_read = read(file, client_message, BUFFER_SIZE)) > 0)
        {
            send(client_sock, client_message, bytes_read, 0);
        }
        close(file);
    }
}

void handle_info(int client_sock, char *file_path_end)
{
    struct stat file_stat;
    char file_path[BUFFER_SIZE - 1024] = "/media/ning/4024-5A83/";
    strcat(file_path, file_path_end);
    char client_message[BUFFER_SIZE];
    printf("File path: %s\n", file_path);
    if (stat(file_path, &file_stat) < 0)
    {
        perror("Error retrieving file information");
        strcpy(client_message, "Error: File not found.");
        send(client_sock, client_message, strlen(client_message), 0);
        return;
    }

    struct passwd *pw = getpwuid(file_stat.st_uid);
    struct group *gr = getgrgid(file_stat.st_gid);
    char time_str[100];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&file_stat.st_mtime));

    char file_info[BUFFER_SIZE];
    snprintf(file_info, sizeof(file_info),
             "Owner: %s\nGroup: %s\nSize: %ld\nLast modified: %s\nPermissions: %04o\n",
             pw->pw_name, gr->gr_name, file_stat.st_size, time_str, file_stat.st_mode & 07777);

    send(client_sock, file_info, strlen(file_info), 0);
}

void handle_md(int client_sock, char *folder_path_end)
{
    char client_message[BUFFER_SIZE];
    char folder_path[BUFFER_SIZE - 1024] = "/media/ning/4024-5A83/";
    strcat(folder_path, folder_path_end);

    int result = mkdir(folder_path, 0755);
    if (result < 0)
    {
        if (result == -1)
        {
            perror("Error creating folder, folder already exists");
            strcpy(client_message, "Error: Folder already exists.");
        }
        else
        {
            perror("Error creating folder");
            strcpy(client_message, "Error: Unable to create folder.");
        }
    }
    else
    {
        snprintf(client_message, sizeof(client_message), "Folder '%s' created successfully.", folder_path);
    }

    send(client_sock, client_message, strlen(client_message), 0);
}

void handle_put(int client_sock, char *file_path_end)
{
    char client_message[BUFFER_SIZE];
    char file_path[BUFFER_SIZE - 1024] = "/media/ning/4024-5A83/";
    strcat(file_path, file_path_end);
    printf("File path: %s\n", file_path);
    int file_desc = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_desc < 0)
    {
        perror("Error creating file");
        strcpy(client_message, "Error: Unable to create file.");
        send(client_sock, client_message, strlen(client_message), 0);
        return;
    }
    printf("File created successfully at server.\n");
    ssize_t bytes_received;
    while ((bytes_received = recv(client_sock, client_message, sizeof(client_message), 0)) > 0)
    {
        write(file_desc, client_message, bytes_received);
        break;
    }
    printf("File uploaded successfully.\n");
    close(file_desc);
    snprintf(client_message, sizeof(client_message), "File '%s' uploaded successfully.", file_path);
    send(client_sock, client_message, strlen(client_message), 0);
}

int main(void)
{
    int socket_desc, client_sock;
    socklen_t client_size;
    struct sockaddr_in server_addr, client_addr;
    char client_message[8196];

    // Clean buffers:
    memset(client_message, '\0', sizeof(client_message));

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0)
    {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // Set the SO_REUSEADDR option:
    int opt = 1;
    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("Error setting socket options");
        return -1;
    }

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("10.211.55.6");

    // Bind to the set port and IP:
    if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");

    // Listen for clients:
    if (listen(socket_desc, 1) < 0)
    {
        printf("Error while listening\n");
        return -1;
    }
    printf("\nListening for incoming connections.....\n");

    // Accept an incoming connection:
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);

    if (client_sock < 0)
    {
        printf("Can't accept\n");
        return -1;
    }
    printf("Client connected at IP: %s and port: %i\n",
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    // Receive client's message:
    if (recv(client_sock, client_message,
             sizeof(client_message), 0) < 0)
    {
        printf("Couldn't receive\n");
        return -1;
    }

    char command[5];
    char file_path[8192];
    sscanf(client_message, "%4s %8191s", command, file_path);
    printf("Command received: %s\n", command);
    if (strcmp(command, "GET") == 0)
    {
        handle_get(client_sock, file_path);
    }
    else if (strcmp(command, "INFO") == 0)
    {
        handle_info(client_sock, file_path);
    }
    else if (strcmp(command, "MD") == 0)
    {
        handle_md(client_sock, file_path);
    }
    else if (strcmp(command, "PUT") == 0)
    {
        printf("PUT command received\n");
        handle_put(client_sock, file_path);
    }
    else
    {
        printf("Invalid command received: %s\n", command);
    }

    // Closing the socket:
    close(client_sock);
    close(socket_desc);

    return 0;
}
