/*
 * @Author: Cao Ning
 * @Date: 2023-01-24 16:09:20
 * @FilePath: polybius.c
 * @Assignment:
 * @Course: CS5600 Spring 2023
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "polybius.h"

#define ROWS 6
#define COLS 6

// Encode the plaintext using the table
void pbEncode(const char *plaintext, char table[6][6])
{
    int i, row, col;
    printf("%s", "The encoded message is: ");
    for (i = 0; i < (int)strlen(plaintext); i++)
    {
        for (row = 0; row < ROWS; row++)
        {
            for (col = 0; col < COLS; col++)
            {
                if (table[row][col] == toupper(plaintext[i]))
                {
                    printf("%d%d", row + 1, col + 1);
                    break;
                }
            }
        }
        // if the character is a space, print a space
        if (plaintext[i] == ' ')
        {
            printf("%s", " ");
        }
    }
    printf("\n");
}

// Decode the ciphertext using the table
void pbDecode(const char *ciphertext, char table[ROWS][COLS])
{
    int i, row, col;
    printf("%s", "The decoded message is: ");
    for (i = 0; i < (int)strlen(ciphertext); i += 2)
    {
        // if the character is a space, print a space
        if (ciphertext[i] == ' ')
        {
            printf("%c", ' ');
            i -= 1;
            continue;
        }
        row = ciphertext[i] - '0' - 1;
        col = ciphertext[i + 1] - '0' - 1;
        printf("%c", table[row][col]);
    }
    printf("\n");
}
