#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_manager.h"
#include "encryption.h"
#include "compression.h"
#include "login.h"

/* uploadFile */
void uploadFile(User* user) {
    char sourcePath[256];
    char destPath[512];
    FILE *sourceFile;

    printf("Enter the path of the file you want to upload: ");
    scanf("%255s", sourcePath);

    sprintf(destPath, "./%s/%s", user->username, strrchr(sourcePath, '/') ? strrchr(sourcePath, '/') + 1 : sourcePath);

    /*Check if user directory exist, if not create one*/
    char command[512];
    sprintf(command, "mkdir -p ./%s", user->username);
    system(command);

    sourceFile = fopen(sourcePath, "rb"); /* Open file in binary mode for reading */
    if (!sourceFile) {
        printf("Error opening source file");
        return;
    }
    /* Compress file */
    compressFile(user, sourceFile, sourcePath);

    printf("File uploaded successfully to %s's directory.\n", user->username);
    printf("Encrypting...\n");

    encryptFile(user, strcat(destPath,".rle"));

    printf("File encrypted successfully.\n");

   
}

/* download */
void downloadFile(User* user) {
    char filename[256];
    char destPath[256];
    char sourcePath[512];
    FILE *sourceFile, *destFile;
    char ch;

    printf("Enter the name of the file you want to download: ");
    scanf("%255s", filename);

    printf("Enter the destination path where you want to save the file: ");
    scanf("%255s", destPath);

    sprintf(sourcePath, "./%s/%s", user->username, filename);



    decryptFile(user, sourcePath);

    sourceFile = fopen(sourcePath, "rb");
    if (!sourceFile) {
        printf("Error opening source file");
        return;
    }

    destFile = fopen(destPath, "wb");
    if (!destFile) {
        fclose(sourceFile);
        printf("Error creating destination file");
        return;
    }

    decompressFile(user, sourceFile, destFile);

    while ((ch = fgetc(sourceFile)) != EOF) {
        fputc(ch, destFile);
    }


    remove(sourcePath);

    fclose(sourceFile);
    fclose(destFile);

    printf("File downloaded successfully to %s.\n", destPath);
}

/* display all files */
void displayFiles(User* user) {
    char command[512];
    char directoryName[256];
    FILE *fp;
    char path[1035];

    sprintf(directoryName, "./%s", user->username);

    /* create the directory, do nothing if directory exists */
    sprintf(command, "mkdir -p %s", directoryName); /* The '-p' flag prevents error if the directory exists */
    system(command);

    /* List files in the directory, output goes to a temporary file */
    sprintf(command, "ls %s > files.txt", directoryName);
    system(command);

    /* Open the file containing the list of files */
    fp = fopen("files.txt", "r");
    if (fp == NULL)
    {
        printf("Failed to run command\n");
        exit(1);
    }

    printf("Files for user %s:\n", user->username);
    /* Read the file line by line to get the names of the files */
    while (fgets(path, sizeof(path), fp) != NULL)
    {
        printf("%s", path);
    }
    /* close the file */
    fclose(fp);

    /* delete the temporary file */
    system("rm files.txt");
}




/* Function to delete a user's file */
void deleteFile(User* user) {
    char filename[256];
    char filepath[512];
    int status;

    printf("Enter the name of the file to delete: ");
    scanf("%255s", filename);

    sprintf(filepath, "./%s/%s", user->username, filename); /* Create the path to the file */

    status = remove(filepath); /* Delete the file */

    if (status == 0) {
        printf("File deleted successfully.\n");
    } else {
        printf("Error deleting the file");
    }
}

