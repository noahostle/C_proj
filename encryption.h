#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#define MAX 20

#include "file_manager.h"

/* Function declarations */
void encryptFile(User* user, char* path);
void decryptFile(User* user, char* path);
void encrypt(char* ptext, char* key, int plen);
void decrypt(char* ptext, char* key, int plen);
void hash(char *str, char *hashed);
void poc();

#endif
