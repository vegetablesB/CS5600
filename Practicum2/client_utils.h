#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024 * 10

void send_to_server(int socket_desc, const char *remote_file_path, const char *command);

void get_local_file_path(char *remote_file_path, char *local_file_path);

void get_remote_file_path(char *remote_file_path, char *local_file_path);
int process_get_command(int socket_desc, const char *local_file_path);
int process_info_md_rm_command(int socket_desc);
int process_put_command(int socket_desc, const char *local_file_path);
int process_command(int argc, char *argv[], int socket_desc);

#endif // CLIENT_UTILS_H
