/*
 * @Author: Cao Ning, Zeng XiangYu
 * @Date: 2023-04-13 17:04:31
 * @FilePath: server.h
 * @Assignment: Practicum II
 * @Course: CS5600 Spring 2023
 */

#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>

#define THREAD_POOL_SIZE 2

typedef struct
{
    char name[10];
    char path[1024];
} USBDrive;

typedef struct
{
    USBDrive usb_drives[2];
    int server_sock;
} Server;

typedef struct
{
    int client_sock;
    char full_path[2048];
} ReadTask;

// Initialize the server, set USB drive names and paths, set socket info
int init_server(Server *server, const char *config_file_path, int port);
// Function to handle GET request
void handle_get(int client_sock, char *file_path_end);
// Function to handle INFO request
void handle_info(int client_sock, char *file_path_end);
// Function to handle MD request
void handle_md(int client_sock, char *folder_path_end);
// Function to handle PUT request
void handle_put(int client_sock, char *file_path_end);
// Function to handle RM request
void handle_rm(int client_sock, char *file_path_end);
// Find the first available thread in the thread pool for parallel GET
int find_available_thread(pthread_t *thread_pool, int size);
// Function to handle parallel GET sending
void *read_and_send_file(void *arg);
// Function to handle parallel GET request
void handle_para_get(int client_sock, const char *file_path);

#endif // SERVER_H
