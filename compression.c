#include <stdio.h>
#include <string.h>
#include "compression.h"
#include "file_manager.h" 

/* compress */
void compressFile(User *user, FILE *readFile, char *readPath)
{
    char writePath[512];
    int c, last, count;
    FILE *writeFile;

    char *filename = strrchr(readPath, '/');

    if (filename == NULL)
    {
        filename = readPath;
    }

    sprintf(writePath, "./%s/%s.rle", user->username, filename); /* The compressed file will have a '.rle' extension */

    writeFile = fopen(writePath, "wb"); /* Open the destination file in binary mode */
    if (!writeFile)
    {
        fclose(readFile);
        printf("Error creating compressed file");
        return;
    }

    last = fgetc(readFile);
    count = 1;
    while ((c = fgetc(readFile)) != EOF)
    {
        if (c == last)
        {
            count++;
            if (count == 255)
            {
                fputc(count, writeFile);
                fputc(last, writeFile);
                count = 0;
            }
        }
        else
        {
            if (count > 0)
            {
                fputc(count, writeFile);
                fputc(last, writeFile);
            }
            last = c;
            count = 1;
        }
    }

    if (count > 0)
    {
        fputc(count, writeFile);
        fputc(last, writeFile);
    }

    fclose(readFile);
    fclose(writeFile);

    printf("File compressed successfully.\n");
}

/* decompress */
void decompressFile(User *user, FILE *readFile, FILE *writeFile)
{

    int c, count;

    while ((count = fgetc(readFile)) != EOF)
    {
        c = fgetc(readFile);
        while (count--)
        {
            fputc(c, writeFile);
        }
    }


    printf("File decompressed successfully.\n");
}
