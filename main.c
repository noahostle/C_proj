/*

Group: 29
Computer lab: 01

Compiling instructions:
run 'make compile' in a terminal and let the makefile do the work for you!
the main executable will be output as 'cloudhosting', and dependencies will be cleaned automatically.

log in or sign up, then upload a file using its absolute path, or just
specify a filename without any path to upload from the project directory.

we have included EXAMPLEFILE for your convenience, so you can simply type
EXAMPLEFILE when prompted for a filename.

files will automatically be compressed and encrypted, then decompressed
and decrypted when you select 'download file', again, if you do not specify
a path, it will automatically save to the project directory under whatever filename
you wrote.

Thanks for your time!!!

*/


#include <stdio.h>
#include <stdlib.h>
#include "file_manager.h"
#include "login.h"

/* display header */
void displayHeader(const char *header)
{
    printf("\033[1;36m"); /* Set text to bold cyan */
    printf("========================================\n");
    printf("%s\n", header);
    printf("========================================\n");
    printf("\033[0m"); /* Reset text formatting */
}

/* display error message */
void displayError(const char *message)
{
    printf("\033[1;31m"); /* Set text to bold red */
    printf("Error: %s\n", message);
    printf("\033[0m"); /* Reset text formatting */
}

int main() {
    User* logged_in_user = NULL;
    char choice;

    displayHeader("Welcome to the cloud hosting system!");

    start();

    do{
        printf("\n");
        if (!logged_in_user) {
            displayHeader("Main Menu");
            printf("Menu: \n");
            printf("1: Sign up\n");
            printf("2: Log in\n");
            printf("0: Exit\n");
            printf("\nEnter your choice: ");
            scanf(" %c", &choice);

            switch (choice) {
            case '1':
                logged_in_user = signup();
                if (logged_in_user) {
                    displayHeader("Signup Successful");
                    printf("\nWelcome, %s!\n", logged_in_user->username);
                }
                break;
            case '2':
                logged_in_user = login();
                if (logged_in_user) {
                    displayHeader("Login Successful");
                    printf("\nWelcome, %s!\n", logged_in_user->username);
                }
                break;
            case '0':
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please choose 1, 2, or 0.\n");
                break;
            }
        } else {
            displayHeader("User Dashboard");
            /* Menu for users who are logged in */
            printf("3: Change password\n");
            printf("4: Display files\n");
            printf("5: Delete file\n");
            printf("6: Upload file\n");
            printf("7: Download file\n");
            printf("8: Log out\n");
            printf("0: Exit\n");
            printf("\nEnter your choice: ");
            scanf(" %c", &choice); 

            switch (choice) {
                case '3':
                    changePassword(logged_in_user);
                    break;
                case '4':
                    displayFiles(logged_in_user);
                    break;
                case '5':
                    deleteFile(logged_in_user);
                    break;
                case '6':
                    uploadFile(logged_in_user);
                    break;
                case '7':
                    downloadFile(logged_in_user);
                    break;
                case '8':
                    printf("Logging out...\n");
                    free(logged_in_user->username);
                    free(logged_in_user->password);
                    logged_in_user = NULL;
                    break;
                case '0':
                    printf("Exiting...\n");
                    break;
                default:
                    printf("Invalid choice. Please choose a number from the menu.\n");
                    break;
            }
        }
    }while (choice != '0');

        /* Free memory */
        if (logged_in_user) {
        free(logged_in_user->username);
        free(logged_in_user->password);
    }

    return 0;
    }
