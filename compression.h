#ifndef COMPRESSION_H
#define COMPRESSION_H

#include "file_manager.h"

#include <stdio.h> 

void compressFile(User *user, FILE *readFile, char *readPath);
void decompressFile(User *user, FILE *readFile, FILE *writeFile);

#endif