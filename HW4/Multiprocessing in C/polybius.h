/*
 * @Author: Cao Ning
 * @Date: 2023-02-01 21:13:23
 * @FilePath: polybius.h
 * @Assignment: HW4 Multiprocessing in C
 * @Course: CS5600 Spring 2023
 */
#ifndef POLYBIUS_H
#define POLYBIUS_H

char *pbEncode(const char *plaintext, char table[6][6]);
char *pbDecode(const char *ciphertext, char table[6][6]);

#endif