#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "login.h"

/* Function declarations */

void uploadFile(User* user);
void downloadFile(User* user);
void displayFiles(User* user);
void deleteFile(User* user);

#endif
