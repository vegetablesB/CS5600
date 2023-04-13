#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>

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

int init_server(Server *server, const char *config_file_path, int port);
void handle_get(int client_sock, char *file_path_end);
void handle_info(int client_sock, char *file_path_end);
void handle_md(int client_sock, char *folder_path_end);
void handle_put(int client_sock, char *file_path_end);
void handle_rm(int client_sock, char *file_path_end);

#endif // SERVER_H
