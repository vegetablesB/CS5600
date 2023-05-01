#ifndef POLYBIUS_H
#define POLYBIUS_H

void pbEncode(const char *plaintext, char table[6][6]);
void pbDecode(const char *ciphertext, char table[6][6]);

#endif