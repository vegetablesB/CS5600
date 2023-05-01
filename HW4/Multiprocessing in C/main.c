/*
 * @Author: Cao Ning
 * @Date: 2023-02-01 21:11:43
 * @FilePath: main.c
 * @Assignment: HW4 Multiprocessing in C
 * @Course: CS5600 Spring 2023
 */
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include "polybius.h"

#define _GNU_SOURCE
#define MAX_SENTENCE_LENGTH 1024

char table[6][6] = {
    {'A', 'B', 'C', 'D', 'E', 'F'},
    {'G', 'H', 'I', 'J', 'K', 'L'},
    {'M', 'N', 'O', 'P', 'Q', 'R'},
    {'S', 'T', 'U', 'V', 'W', 'X'},
    {'Y', 'Z', '0', '1', '2', '3'},
    {'4', '5', '6', '7', '8', '9'}};

struct Sentence
{
    char sentence[MAX_SENTENCE_LENGTH];
    int sentence_num;
    char *file_name;
};

char *generate_file_name()
{
    time_t raw_time;
    struct tm *time_info;
    char *file_name = (char *)malloc(sizeof(char) * 50);
    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(file_name, 50, "output_%Y-%m-%d_%H-%M-%S.txt", time_info);
    return file_name;
}

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// print decoded sentence to a file named by timestamp
void *cipher(void *sentence_struct)
{

    struct Sentence *s = (struct Sentence *)sentence_struct;

    char *ciphertext = pbEncode(s->sentence, table);
    pthread_mutex_lock(&lock);
    FILE *fp = fopen(s->file_name, "a");
    if (fp == NULL)
    {
        printf("Error opening file %s\n", s->file_name);
        pthread_mutex_unlock(&lock);
        return NULL;
    }

    fprintf(fp, "%s.\n", ciphertext);
    fclose(fp);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char *argv[])
{
    // check if the user input is valid
    if (argc > 3 || argc < 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        printf("Usage: %s -h for help\n", argv[0]);
        return 1;
    }

    if (argc == 2 && strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        printf("Usage: %s <input_file> -d for detailed ouput\n", argv[0]);
        return 1;
    }

    // read file
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("Error opening file %s\n", argv[1]);
        return 1;
    }
    // read sentences
    char sentence[MAX_SENTENCE_LENGTH];
    int sentence_num = 0;
    char *file_name = generate_file_name();
    pthread_t threads[100];
    while (fscanf(fp, " %1000[^.?!]", sentence) != EOF)
    {
        sentence_num++;
        if (argc == 3 && strcmp(argv[1], "-d") != 0)
        {
            printf("Sentence %d: %s\n", sentence_num, sentence);
        }

        struct Sentence *s = (struct Sentence *)malloc(sizeof(struct Sentence));
        strcpy(s->sentence, sentence);
        s->sentence_num = sentence_num;
        s->file_name = file_name;
        int ret = pthread_create(&threads[sentence_num - 1], NULL, cipher, (void *)s);
        if (ret != 0)
        {
            printf("Error creating thread %d\n", sentence_num);
            return 1;
        }
        fgetc(fp); // to skip the comma
    }
    fclose(fp);

    printf("Number of sentences: %d\n", sentence_num);
    if (sentence_num == 0)
    {
        printf("No sentence found in the file %s\n", argv[1]);
        return 1;
    }

    for (int i = 0; i < sentence_num; i++)
    {
        // printf("Thread %lu is running\n", threads[i]);
        pthread_join(threads[i], NULL);
    }

    printf("Encoded sentences are written to %s\n", file_name);
    return 0;
}


// They treat me as cold ice. But still thank them making me thinking further.
