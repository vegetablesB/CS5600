/*
 * @Author: Cao Ning
 * @Date: 2023-01-20 22:14:00
 * @FilePath: cipher.c
 * @Assignment:
 * @Course: CS5600 Spring 2023
 */
#include <stdio.h>
#include <string.h>
#include "polybius.h"
int n = 10;
int main(int argc, char *argv[])
{
    // create the table including the alphabet, numbers.
    char my_table[6][6] = {
        {'A', 'B', 'C', 'D', 'E', 'F'},
        {'G', 'H', 'I', 'J', 'K', 'L'},
        {'M', 'N', 'O', 'P', 'Q', 'R'},
        {'S', 'T', 'U', 'V', 'W', 'X'},
        {'Y', 'Z', '0', '1', '2', '3'},
        {'4', '5', '6', '7', '8', '9'}};
    // check if the user input is valid
    if (argc < 3 || argc > 3)
    {
        printf("Usage for encode: %s -e \"text\"\n", argv[0]);
        printf("Usage for decode: %s -d \"text\"\n", argv[0]);
        if (argc > 3)
        {
            printf("Too many arguments supplied. Put text in to \"\"\n");
        }
        return 1;
    }
    if (strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-d") != 0)
    {
        printf("Invalid option: %s\n", argv[1]);
        printf("Usage for encode: %s -e \"text\"\n", argv[0]);
        printf("Usage for decode: %s -d \"text\"\n", argv[0]);
        return 1;
    }
    // encode the message
    if (strcmp(argv[1], "-e") == 0)
    {
        char *str = argv[2];
        pbEncode(str, my_table);
        return 0;
    }
    /// decode the message
    if (strcmp(argv[1], "-d") == 0)
    {
        char *str = argv[2];
        pbDecode(str, my_table);
        return 0;
    }
}