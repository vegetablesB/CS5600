/*
 * @Author: Cao Ning
 * @Date: 2023-02-01 21:13:23
 * @FilePath: polybius.c
 * @Assignment: HW4 Multiprocessing in C
 * @Course: CS5600 Spring 2023
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "polybius.h"

#define ROWS 6
#define COLS 6

// Encode the plaintext using the table
char *pbEncode(const char *plaintext, char table[6][6])
{
    int i, row, col;
    char *ciphertext = (char *)malloc(sizeof(char) * strlen(plaintext) * 3);
    int j = 0;
    for (i = 0; i < (int)strlen(plaintext); i++)
    {
        for (row = 0; row < ROWS; row++)
        {
            for (col = 0; col < COLS; col++)
            {
                if (table[row][col] == toupper(plaintext[i]))
                {

                    ciphertext[j] = '0' + row + 1;
                    ciphertext[j + 1] = '0' + col + 1;
                    j += 2;
                    break;
                }
            }
        }
        // if the character is not a letter
        if (!isalpha(plaintext[i]))
        {
            ciphertext[j] = plaintext[i];
            j += 1;
        }
    }
    ciphertext[j] = '\0';

    return ciphertext;
}

// Decode the ciphertext using the table
char *pbDecode(const char *ciphertext, char table[ROWS][COLS])
{
    int i, row, col;
    char *decoded = (char *)malloc(sizeof(char) * strlen(ciphertext));
    int j = 0;
    for (i = 0; i < (int)strlen(ciphertext); i += 2)
    {
        // if the character is a space, print a space
        if (!isdigit(ciphertext[i]))
        {
            decoded[j] = ciphertext[i];
            i -= 1;
            j += 1;
            continue;
        }
        row = ciphertext[i] - '0' - 1;
        col = ciphertext[i + 1] - '0' - 1;
        decoded[j] = table[row][col];
        j += 1;
    }
    decoded[j] = '\0';
    return decoded;
}
