#include "client_utils.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void send_to_server(int socket_desc, const char *remote_file_path, const char *command)
{
    char client_message[BUFFER_SIZE];
    memset(client_message, '\0', sizeof(client_message));
    snprintf(client_message, sizeof(client_message), "%s %s", command, remote_file_path);
    printf("Sending message (length: %zu): %s\n", strlen(client_message), client_message);
    // Send one more byte for the null terminator, slove the problem the server got a wrong client_message
    if (send(socket_desc, client_message, strlen(client_message) + 1, 0) < 0)
    {
        perror("Unable to send message");
    }
}

void get_local_file_path(char *remote_file_path, char *local_file_path)
{
    // Set the directory path for the local file
    strcpy(local_file_path, "/home/ning/Workspace/HW12/");

    // Find the last slash in the remote file path
    char *last_slash = strrchr(remote_file_path, '/');
    if (last_slash == NULL)
    {
        // No slash found, so just use the filename as-is
        strcat(local_file_path, remote_file_path);
    }
    else
    {
        // Append the filename to the directory path
        strcat(local_file_path, last_slash + 1);
    }
}

void get_remote_file_path(char *remote_file_path, char *local_file_path)
{
    // Set the directory path for the local file
    strcpy(remote_file_path, "");

    // Find the last slash in the remote file path
    char *last_slash = strrchr(local_file_path, '/');
    if (last_slash == NULL)
    {
        // No slash found, so just use the filename as-is
        strcat(remote_file_path, local_file_path);
    }
    else
    {
        // Append the filename to the directory path
        strcat(remote_file_path, last_slash + 1);
    }
}

int process_get_command(int socket_desc, const char *local_file_path)
{
    char server_message[BUFFER_SIZE - 1024];
    memset(server_message, '\0', sizeof(server_message));
    // Open local file for writing:
    int local_file = open(local_file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (local_file < 0)
    {
        perror("Error opening local file");
        return -1;
    }

    // Receive the server's response:
    ssize_t bytes_received;
    while ((bytes_received = recv(socket_desc, server_message, sizeof(server_message), 0)) > 0)
    {
        write(local_file, server_message, bytes_received);
    }

    if (bytes_received < 0)
    {
        perror("Error while receiving server's msg");
        close(local_file);
        return -1;
    }

    // Close the local file and socket:
    close(local_file);
    printf("File received and saved as: %s\n", local_file_path);
    return 0;
}

int process_info_md_rm_command(int socket_desc)
{
    char info[BUFFER_SIZE];
    ssize_t bytes_received;
    while ((bytes_received = recv(socket_desc, info, sizeof(info), 0)) > 0)
    {
        fwrite(info, 1, bytes_received, stdout);
    }
    printf("\n");
    return 0;
}

int process_put_command(int socket_desc, const char *local_file_path)
{
    char server_message[BUFFER_SIZE - 1024];
    memset(server_message, '\0', sizeof(server_message));

    // Open the local file for reading
    int file_desc = open(local_file_path, O_RDONLY);
    if (file_desc < 0)
    {
        perror("Error opening file");
        return -1;
    }

    // Read the local file and send its contents to the server
    char client_message[BUFFER_SIZE];
    memset(client_message, '\0', sizeof(client_message));
    ssize_t bytes_read;
    while ((bytes_read = read(file_desc, client_message, BUFFER_SIZE)) > 0)
    {
        send(socket_desc, client_message, bytes_read, 0);
    }
    close(file_desc); // Close the local file descriptor

    // Shutdown the sending side of the connection
    if (shutdown(socket_desc, SHUT_WR) < 0)
    {
        perror("Error shutting down the sending side of the connection");
        return -1;
    }

    // Receive the server's response
    if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0)
    {
        printf("Error while receiving server's msg\n");
        return -1;
    }
    printf("Server's response: %s\n", server_message);
    return 0;
}

int process_command(int argc, char *argv[], int socket_desc)
{
    char remote_file_path[BUFFER_SIZE], local_file_path[BUFFER_SIZE];

    // Clean buffers:
    memset(remote_file_path, '\0', sizeof(remote_file_path));
    memset(local_file_path, '\0', sizeof(local_file_path));

    // Send the command and file path to server:

    if (strcmp(argv[1], "GET") == 0)
    {
        strcpy(remote_file_path, argv[2]);
        if (argc == 4)
        {
            strcpy(local_file_path, argv[3]);
        }
        else
        {
            get_local_file_path(remote_file_path, local_file_path);
        }
        printf("Local file path: %s\n", local_file_path);
        printf("Remote file path: %s\n", remote_file_path);

        send_to_server(socket_desc, remote_file_path, argv[1]);

        return process_get_command(socket_desc, local_file_path);
    }
    else if (strcmp(argv[1], "INFO") == 0)
    {
        strcpy(remote_file_path, argv[2]);
        send_to_server(socket_desc, remote_file_path, argv[1]);

        return process_info_md_rm_command(socket_desc);
    }
    else if (strcmp(argv[1], "MD") == 0)
    {
        strcpy(remote_file_path, argv[2]);
        send_to_server(socket_desc, remote_file_path, argv[1]);

        return process_info_md_rm_command(socket_desc);
    }
    else if (strcmp(argv[1], "RM") == 0)
    {
        strcpy(remote_file_path, argv[2]);
        send_to_server(socket_desc, remote_file_path, argv[1]);

        return process_info_md_rm_command(socket_desc);
    }

    else if (strcmp(argv[1], "PUT") == 0)
    {

        strcpy(local_file_path, argv[2]);
        if (argc == 4)
        {
            strcpy(remote_file_path, argv[3]);
        }
        else
        {
            get_remote_file_path(remote_file_path, local_file_path);
        }
        printf("remote file path: %s\n", remote_file_path);
        // Send the command, local file path, and remote file path to server
        send_to_server(socket_desc, remote_file_path, argv[1]);
        return process_put_command(socket_desc, local_file_path);
    }
    else
    {
        printf("Invalid command: %s\n", argv[1]);
        return -1;
    }
    return 0;
}
