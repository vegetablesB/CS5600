/*
 * @Author: Cao Ning, Zeng XiangYu
 * @Date: 2023-04-13 16:31:20
 * @FilePath: server.c
 * @Assignment: Practicum II
 * @Course: CS5600 Spring 2023
 */

#include "server.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#include <errno.h>

#define BUFFER_SIZE 1024 * 10
// Global variable to control the server loop
volatile int running = 1;
Server server;
pthread_mutex_t file_system_mutex;
pthread_t thread_pool[THREAD_POOL_SIZE] = {0};

void permission_string(mode_t mode, char *str)
{
    str[0] = (S_ISDIR(mode)) ? 'd' : '-';
    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    str[3] = (mode & S_IXUSR) ? 'x' : '-';
    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    str[6] = (mode & S_IXGRP) ? 'x' : '-';
    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    str[9] = (mode & S_IXOTH) ? 'x' : '-';
    str[10] = '\0';
}

int init_server(Server *server, const char *config_file_path, int port)
{
    // Initialize mutex
    if (pthread_mutex_init(&file_system_mutex, NULL) != 0)
    {
        perror("Error initializing file system mutex");
        return -1;
    }
    // Read USB device paths from the config file
    FILE *config_file = fopen(config_file_path, "r");
    if (config_file == NULL)
    {
        perror("Error opening config file");
        return -1;
    }
    for (int i = 0; i < 2; i++)
    {
        fscanf(config_file, "%[^:]:%s\n", server->usb_drives[i].name, server->usb_drives[i].path);
    }
    fclose(config_file);

    // Create socket
    server->server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_sock < 0)
    {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    int enable = 1;
    if (setsockopt(server->server_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
    {
        perror("setsockopt(SO_REUSEADDR) failed");
        return -1;
    }
    // Set port and IP
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind to the set port and IP
    if (bind(server->server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");

    // Listen for clients
    if (listen(server->server_sock, 1) < 0)
    {
        printf("Error while listening\n");
        return -1;
    }
    printf("\nListening for incoming connections...\n");

    return 0;
}

void handle_get(int client_sock, char *file_path_end)
{
    pthread_mutex_lock(&file_system_mutex);
    int file;
    char client_message[BUFFER_SIZE];
    ssize_t bytes_read;
    char file_path1[BUFFER_SIZE - 1024];
    strcpy(file_path1, server.usb_drives[0].path);
    strcat(file_path1, file_path_end);
    printf("File path1: %s\n", file_path1);

    char file_path2[BUFFER_SIZE - 1024];
    strcpy(file_path2, server.usb_drives[1].path);
    strcat(file_path2, file_path_end);
    printf("File path2: %s\n", file_path2);

    // Try to open the file on the first USB device
    file = open(file_path1, O_RDONLY);
    if (file < 0)
    {
        perror("Error opening file on USB device 1");

        // If the file was not found on the first USB device, try the second one
        file = open(file_path2, O_RDONLY);
        if (file < 0)
        {
            perror("Error opening file on USB device 2");
            strcpy(client_message, "Error: File not found.");
            send(client_sock, client_message, strlen(client_message), 0);
            pthread_mutex_unlock(&file_system_mutex);
            return;
        }
    }

    // Send the file content to the client
    while ((bytes_read = read(file, client_message, BUFFER_SIZE)) > 0)
    {
        send(client_sock, client_message, bytes_read, 0);
        memset(client_message, '\0', sizeof(client_message));
    }
    close(file);
    pthread_mutex_unlock(&file_system_mutex);
}

void handle_info(int client_sock, char *file_path_end)
{
    pthread_mutex_lock(&file_system_mutex);
    struct stat file_stat;
    char file_path[BUFFER_SIZE - 1024] = "/media/ning/4024-5A83/";
    strcat(file_path, file_path_end);

    printf("File path: %s\n", file_path);
    if (stat(file_path, &file_stat) < 0)
    {
        char client_message[BUFFER_SIZE];
        perror("Error retrieving file information");
        strcpy(client_message, "Error: File not found.");
        send(client_sock, client_message, strlen(client_message), 0);
        pthread_mutex_unlock(&file_system_mutex);
        return;
    }

    struct passwd *pw = getpwuid(file_stat.st_uid);
    struct group *gr = getgrgid(file_stat.st_gid);
    char time_str[100];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&file_stat.st_mtime));
    char permissions[11];
    permission_string(file_stat.st_mode, permissions);
    char file_info[BUFFER_SIZE];
    snprintf(file_info, sizeof(file_info), "%s|%s|%s|%lld|%s", pw->pw_name, gr->gr_name, permissions, (long long)file_stat.st_size, time_str);
    printf("File info: %s\n", file_info);
    send(client_sock, file_info, strlen(file_info), 0);
    pthread_mutex_unlock(&file_system_mutex);
}

void handle_md(int client_sock, char *folder_path_end)
{
    pthread_mutex_lock(&file_system_mutex);
    char client_message1[BUFFER_SIZE];
    char client_message2[BUFFER_SIZE];

    char folder_path1[BUFFER_SIZE - 1024];
    strcpy(folder_path1, server.usb_drives[0].path);
    strcat(folder_path1, folder_path_end);
    printf("Folder path1: %s\n", folder_path1);

    char folder_path2[BUFFER_SIZE - 1024];
    strcpy(folder_path2, server.usb_drives[1].path);
    strcat(folder_path2, folder_path_end);
    printf("Folder path2: %s\n", folder_path2);

    int result1 = mkdir(folder_path1, 0755);
    if (result1 < 0)
    {
        if (errno == EEXIST)
        {
            perror("Error creating folder, folder already exists. ");
            strcpy(client_message1, "Error: Folder already exists.");
        }
        else
        {
            perror("Error creating folder");
            strcpy(client_message1, "Error: Unable to create folder.");
        }
    }
    else if (result1 == 0)
    {
        printf("Folder %s created successfully.\n", folder_path1);
        snprintf(client_message1, sizeof(client_message1), "Folder '%s' created successfully.\n", folder_path1);
    }

    int result2 = mkdir(folder_path2, 0755);
    if (result2 < 0)
    {
        if (errno == EEXIST)
        {
            perror("Error creating folder, folder already exists. ");
            strcpy(client_message2, "Error: Folder already exists.");
        }
        else
        {
            perror("Error creating folder");
            strcpy(client_message2, "Error: Unable to create folder.");
        }
    }
    else if (result2 == 0)
    {
        printf("Folder %s created successfully.\n", folder_path2);
        snprintf(client_message2, sizeof(client_message2), "Folder '%s' created successfully.\n", folder_path2);
    }

    send(client_sock, client_message1, strlen(client_message1), 0);
    send(client_sock, client_message2, strlen(client_message2), 0);
    pthread_mutex_unlock(&file_system_mutex);
}

void handle_rm(int client_sock, char *file_path_end)
{
    pthread_mutex_lock(&file_system_mutex);
    char client_message1[BUFFER_SIZE];
    char client_message2[BUFFER_SIZE];
    char file_path1[BUFFER_SIZE - 1024];
    strcpy(file_path1, server.usb_drives[0].path);
    strcat(file_path1, file_path_end);
    printf("File path1: %s\n", file_path1);

    char file_path2[BUFFER_SIZE - 1024];
    strcpy(file_path2, server.usb_drives[1].path);
    strcat(file_path2, file_path_end);
    printf("File path2: %s\n", file_path2);

    struct stat path_stat1;
    struct stat path_stat2;
    if (stat(file_path1, &path_stat1) < 0 && stat(file_path2, &path_stat2) < 0)
    {
        perror("Error checking file status");
        strcpy(client_message1, "Error: File or folder does not exist.");
        send(client_sock, client_message1, strlen(client_message1), 0);
        pthread_mutex_unlock(&file_system_mutex);
        return;
    }

    int result1;
    if (S_ISDIR(path_stat1.st_mode))
    {
        result1 = rmdir(file_path1);
        if (result1 == 0)
        {
            snprintf(client_message1, sizeof(client_message1), "Folder '%s' deleted successfully.\n", file_path1);
        }
        else
        {
            perror("Error deleting folder");
            snprintf(client_message1, sizeof(client_message1), "Error: Unable to delete folder '%s'.\n", file_path1);
        }
    }
    else
    {
        result1 = remove(file_path1);
        if (result1 == 0)
        {
            snprintf(client_message1, sizeof(client_message1), "File '%s' deleted successfully.\n", file_path1);
        }
        else
        {
            perror("Error deleting file");
            snprintf(client_message1, sizeof(client_message1), "Error: Unable to delete file '%s'.\n", file_path1);
        }
    }

    int result2;
    if (S_ISDIR(path_stat2.st_mode))
    {
        result2 = rmdir(file_path2);
        if (result2 == 0)
        {
            snprintf(client_message2, sizeof(client_message2), "Folder '%s' deleted successfully.\n", file_path2);
        }
        else
        {
            perror("Error deleting folder");
            snprintf(client_message2, sizeof(client_message2), "Error: Unable to delete folder '%s'.\n", file_path2);
        }
    }
    else
    {
        result2 = remove(file_path2);
        if (result2 == 0)
        {
            snprintf(client_message2, sizeof(client_message2), "File '%s' deleted successfully.\n", file_path2);
        }
        else
        {
            perror("Error deleting file");
            snprintf(client_message2, sizeof(client_message2), "Error: Unable to delete file '%s'.\n", file_path2);
        }
    }

    send(client_sock, client_message1, strlen(client_message1), 0);
    send(client_sock, client_message2, strlen(client_message2), 0);
    pthread_mutex_unlock(&file_system_mutex);
}

void handle_put(int client_sock, char *file_path_end)
{
    pthread_mutex_lock(&file_system_mutex);
    char client_message[BUFFER_SIZE];
    memset(client_message, '\0', sizeof(client_message));
    ssize_t bytes_received;

    char file_path1[BUFFER_SIZE - 1024];
    strcpy(file_path1, server.usb_drives[0].path);
    strcat(file_path1, file_path_end);
    printf("File path1: %s\n", file_path1);

    char file_path2[BUFFER_SIZE - 1024];
    strcpy(file_path2, server.usb_drives[1].path);
    strcat(file_path2, file_path_end);
    printf("File path2: %s\n", file_path2);

    int file_desc1 = open(file_path1, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int file_desc2 = open(file_path2, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (file_desc1 < 0 && file_desc2 < 0)
    {
        perror("Error creating files on both USB devices");
        strcpy(client_message, "Error: Unable to create file on both USB devices.");
        send(client_sock, client_message, strlen(client_message), 0);
        pthread_mutex_unlock(&file_system_mutex);
        return;
    }
    char file_data[BUFFER_SIZE];
    memset(file_data, '\0', sizeof(file_data));

    int eof_received = 0;

    while (!eof_received && (bytes_received = recv(client_sock, file_data, sizeof(file_data), 0)) > 0)
    {
        if (strcmp(file_data, "EOF") == 0)
        {
            eof_received = 1;
            break;
        }
        if (file_desc1 >= 0)
        {
            write(file_desc1, file_data, bytes_received);
        }
        if (file_desc2 >= 0)
        {
            write(file_desc2, file_data, bytes_received);
        }
        memset(file_data, '\0', sizeof(file_data)); // Clear the buffer for the next iteration
    }
    memset(file_data, '\0', sizeof(file_data));
    if (file_desc1 >= 0)
    {
        printf("File uploaded successfully to USB device 1.\n");
        close(file_desc1);
    }
    if (file_desc2 >= 0)
    {
        printf("File uploaded successfully to USB device 2.\n");
        close(file_desc2);
    }

    snprintf(client_message, sizeof(client_message), "File '%s' uploaded successfully.", file_path_end);
    send(client_sock, client_message, strlen(client_message), 0);
    pthread_mutex_unlock(&file_system_mutex);
}

int find_available_thread(pthread_t *thread_pool, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (thread_pool[i] == 0 ||                // Check if the thread has not been started yet
            pthread_kill(thread_pool[i], 0) != 0) // Check if the thread has terminated
        {
            return i;
        }
    }

    return -1;
}

void *read_and_send_file(void *arg)
{
    ReadTask *task = (ReadTask *)arg;
    int client_sock = task->client_sock;
    char *full_path = task->full_path;

    int file;
    char client_message[BUFFER_SIZE];

    ssize_t bytes_read;

    // Read the file from task->full_path and send its content to the client using task->client_sock
    // Try to open the file on the first USB device
    file = open(full_path, O_RDONLY);
    if (file < 0)
    {
        perror("Error opening file on USB device");
    }
    // Send the file content to the client
    while ((bytes_read = read(file, client_message, BUFFER_SIZE)) > 0)
    {
        ssize_t bytes_sent = send(client_sock, client_message, bytes_read, 0);

        if (bytes_sent < 0)
        {
            perror("Error sending data to the client");
            break;
        }
    }
    close(file);
    free(task);
    close(client_sock);
    return NULL;
}

void handle_para_get(int client_sock, const char *file_path)
{

    // Read the file from the corresponding USB device
    int thread_index = find_available_thread(thread_pool, THREAD_POOL_SIZE);
    // int thread_index = 0;
    printf("Thread %d is available\n", thread_index);
    if (thread_index == -1)
    {
        // Handle error: all threads are busy
        perror("Error: All threads are busy");
        char client_message[BUFFER_SIZE];
        strcpy(client_message, "Error: All threads are busy, can not do parallel read. Please try again later.");
        send(client_sock, client_message, strlen(client_message), 0);
        return;
    }
    else
    {
        int client_sock_copy = dup(client_sock);
        ReadTask *task = (ReadTask *)malloc(sizeof(ReadTask));

        task->client_sock = client_sock_copy;
        // set full path use server.usb_drives[thread_index].path and file_path, to task->full_path
        snprintf(task->full_path, 2048, "%s%s", server.usb_drives[thread_index].path, file_path);

        // Assign the task to the thread
        pthread_create(&thread_pool[thread_index], NULL, read_and_send_file, task);
    }
}

void sigint_handler(int sig)
{
    running = 0;
}

void *handle_client(void *arg)
{
    int client_sock = (int)(intptr_t)arg;
    char command_message[BUFFER_SIZE];
    // Clean buffers:
    memset(command_message, '\0', sizeof(command_message));

    // Receive client's message:
    // sometimes the bytes_received is 58 which is larger than the size of the message (13) sent by the client
    ssize_t bytes_received = recv(client_sock, command_message, sizeof(command_message), 0);
    if (bytes_received < 0)
    {
        printf("Couldn't receive\n");
        return NULL;
    }

    char command[10];
    char file_path[100];
    memset(command, '\0', sizeof(command));
    memset(file_path, '\0', sizeof(file_path));
    sscanf(command_message, "%9s %99s", command, file_path);

    // Send acknowledgement to the client
    // Handshake first. If don't, the client will send the file before the server is ready to receive it.
    int optval = 1;
    setsockopt(client_sock, SOL_SOCKET, SO_SNDBUF, &optval, sizeof(optval));
    char ack_msg[10];
    memset(ack_msg, '\0', sizeof(ack_msg));
    strcpy(ack_msg, "ACK");

    send(client_sock, ack_msg, strlen(ack_msg), 0);

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
        handle_put(client_sock, file_path);
    }
    else if (strcmp(command, "RM") == 0)
    {
        handle_rm(client_sock, file_path);
    }
    else if (strcmp(command, "PARAGET") == 0)
    {
        handle_para_get(client_sock, file_path);
    }
    else
    {
        printf("Invalid command received: %s\n", command);
    }
    // Closing the socket:
    close(client_sock);

    pthread_exit(NULL);
}

int main(void)
{

    // Initialize server: read config file and create socket:
    if (init_server(&server, "config.txt", 2000) < 0)
    {
        printf("Failed to initialize server\n");
        return -1;
    }

    // Register signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, sigint_handler);

    while (running)
    {

        // Accept an incoming connection
        socklen_t client_size;
        struct sockaddr_in client_addr;
        client_size = sizeof(client_addr);
        int client_sock = accept(server.server_sock, (struct sockaddr *)&client_addr, &client_size);
        if (client_sock < 0)
        {
            printf("Can't accept\n");
            return -1;
        }
        printf("\nClient connected at IP: %s and port: %i\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
        // Create a new thread to handle the client connection
        pthread_t client_thread;

        if (pthread_create(&client_thread, NULL, handle_client, (void *)(intptr_t)client_sock) != 0)
        {
            perror("Error creating client thread");
            close(client_sock);
            continue;
        }

        // Detach the thread to let it clean up when it's finished
        pthread_detach(client_thread);
    }

    close(server.server_sock);

    return 0;
}
