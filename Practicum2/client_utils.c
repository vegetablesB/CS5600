#include "client_utils.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void send_to_server(int socket_desc, char *client_message, size_t buffer_size, char *remote_file_path, char *command)
{
    // clear the buffer, solve the problem of the server receiving extra characters
    memset(client_message, '\0', buffer_size);
    snprintf(client_message, buffer_size, "%s %s", command, remote_file_path);
    printf("%s\n", client_message);
    printf("Sending message (length: %zu): %s\n", strlen(client_message), client_message);

    if (send(socket_desc, client_message, strlen(client_message), 0) < 0)
    {
        printf("Unable to send message)\n");
        return;
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

int process_command(int argc, char *argv[], int socket_desc)
{
    char remote_file_path[8192], local_file_path[8192];
    char server_message[BUFFER_SIZE - 1024], client_message[BUFFER_SIZE - 1024];

    // Clean buffers:
    memset(remote_file_path, '\0', sizeof(remote_file_path));
    memset(local_file_path, '\0', sizeof(local_file_path));
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

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
        printf("local file path: %s\n", local_file_path);
        printf("remote file path: %s\n", remote_file_path);
        send_to_server(socket_desc, client_message, sizeof(client_message), remote_file_path, argv[1]);
        // ... handle GET command as before ...
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
            printf("Error while receiving server's msg\n");
            close(local_file);
            return -1;
        }

        // Close the local file and socket:
        close(local_file);
        printf("File received and saved as: %s\n", local_file_path);
    }
    else if (strcmp(argv[1], "INFO") == 0)
    {
        strcpy(remote_file_path, argv[2]);
        send_to_server(socket_desc, client_message, sizeof(client_message), remote_file_path, argv[1]);
        printf("File info:\n");
        char file_info[BUFFER_SIZE];
        ssize_t bytes_received;
        while ((bytes_received = recv(socket_desc, file_info, sizeof(file_info), 0)) > 0)
        {
            fwrite(file_info, 1, bytes_received, stdout);
        }
        printf("\n");
    }
    else if (strcmp(argv[1], "MD") == 0)
    {
        strcpy(remote_file_path, argv[2]);
        send_to_server(socket_desc, client_message, sizeof(client_message), remote_file_path, argv[1]);
        char status[BUFFER_SIZE];
        ssize_t bytes_received;
        while ((bytes_received = recv(socket_desc, status, sizeof(status), 0)) > 0)
        {
            fwrite(status, 1, bytes_received, stdout);
        }
        printf("\n");
    }
    else if (strcmp(argv[1], "RM") == 0)
    {
        strcpy(remote_file_path, argv[2]);
        send_to_server(socket_desc, client_message, sizeof(client_message), remote_file_path, argv[1]);
        char status[BUFFER_SIZE];
        ssize_t bytes_received;
        while ((bytes_received = recv(socket_desc, status, sizeof(status), 0)) > 0)
        {
            fwrite(status, 1, bytes_received, stdout);
        }
        printf("\n");
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
        send_to_server(socket_desc, client_message, sizeof(client_message), remote_file_path, argv[1]);
        // Open the local file for reading
        int file_desc = open(local_file_path, O_RDONLY);
        if (file_desc < 0)
        {
            perror("Error opening file");
            return -1;
        }

        // Read the local file and send its contents to the server
        ssize_t bytes_read;
        while ((bytes_read = read(file_desc, client_message, sizeof(client_message))) > 0)
        {
            send(socket_desc, client_message, bytes_read, 0);
        }
        close(file_desc); // Receive the server's response
        if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0)
        {
            printf("Error while receiving server's msg\n");
            return -1;
        }
        printf("Server's response: %s\n", server_message);
    }
    else
    {
        printf("Invalid command: %s\n", argv[1]);
        return -1;
    }
    return 0;
}