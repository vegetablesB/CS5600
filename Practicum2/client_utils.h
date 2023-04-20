/*
 * @Author: Cao Ning, Zeng XiangYu
 * @Date: 2023-04-13 21:17:33
 * @FilePath: client_utils.h
 * @Assignment: Practicum II
 * @Course: CS5600 Spring 2023
 */

#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024 * 10

// Show loading animation
void show_loading_animation(int duration_ms);
// Send remote file path to server and establish connection
void send_to_server(int socket_desc, const char *remote_file_path, const char *command);
// Get local file path for missing command
void get_local_file_path(char *remote_file_path, char *local_file_path);
// Get remote file path for missing command
void get_remote_file_path(char *remote_file_path, char *local_file_path);
// Function for GET command
int process_get_command(int socket_desc, const char *local_file_path);
// Function for INFO MD RM command
int process_info_md_rm_command(int socket_desc);
// Function for PUT command
int process_put_command(int socket_desc, const char *local_file_path);
// Function for command: set path and run command function
int process_command(int argc, char *argv[], int socket_desc);

#endif // CLIENT_UTILS_H
