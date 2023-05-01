/*
 * @Author: Cao Ning, Zeng XiangYu
 * @Date: 2023-04-17 14:04:26
 * @FilePath: multi_client_test.c
 * @Assignment: Practicum II
 * @Course: CS5600 Spring 2023
 */

#include "client_utils.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define NUM_THREADS 5

typedef struct
{
    int thread_id;
    char *file_path;
} ClientThreadArg;

void *client_thread_function(void *arg)
{
    ClientThreadArg *client_arg = (ClientThreadArg *)arg;

    int sock;
    struct sockaddr_in server;
    char message[BUFFER_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Could not create socket");
        pthread_exit(NULL);
    }

    server.sin_addr.s_addr = inet_addr("10.211.55.6");
    server.sin_family = AF_INET;
    server.sin_port = htons(2000);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Connect failed. Error");
        pthread_exit(NULL);
    }

    // Send the GET request
    snprintf(message, BUFFER_SIZE, "GET multi_client/example%d.txt", client_arg->thread_id);
    if (send(sock, message, strlen(message), 0) < 0)
    {
        perror("Send failed");
        pthread_exit(NULL);
    }

    recv_ack(sock);

    // Receive the response and save it to a file
    char file_name[BUFFER_SIZE];
    snprintf(file_name, BUFFER_SIZE, "../Multi_client_test/output_%d.txt", client_arg->thread_id);
    FILE *output_file = fopen(file_name, "w");
    if (!output_file)
    {
        perror("Error opening output file");
        pthread_exit(NULL);
    }

    ssize_t received_bytes;
    while ((received_bytes = recv(sock, message, BUFFER_SIZE, 0)) > 0)
    {
        // remove first three characters

        fwrite(message, 1, received_bytes, output_file);
    }

    fclose(output_file);
    close(sock);

    printf("Thread %d: Received and saved data to '%s'\n", client_arg->thread_id, file_name);

    pthread_exit(NULL);
}

int main(void)
{
    pthread_t threads[NUM_THREADS];
    ClientThreadArg thread_args[NUM_THREADS];
    char file_path[25] = "multi_client/example.txt";

    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_args[i].thread_id = i + 1;
        thread_args[i].file_path = file_path;

        if (pthread_create(&threads[i], NULL, client_thread_function, (void *)&thread_args[i]) != 0)
        {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
