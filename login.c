#include "login.h"
#include "encryption.h"
#include "file_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AVLNode
{
    char username[101];
    char hashedpassword[MAX + sizeof(char)];
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

AVLNode *root = NULL;

int height(AVLNode *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

AVLNode *newNode(char *username, char *hashedpassword)
{
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    strcpy(node->username, username);
    strcpy(node->hashedpassword, hashedpassword);
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode *rightRotate(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode *leftRotate(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(AVLNode *node)
{
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

AVLNode *insert(AVLNode *node, char *username, char *hashedpassword)
{
    if (node == NULL)
        return newNode(username, hashedpassword);

    if (strcmp(username, node->username) < 0)
    {
        node->left = insert(node->left, username, hashedpassword);
    }
    else if (strcmp(username, node->username) > 0)
    {
        node->right = insert(node->right, username, hashedpassword);
    }
    else
    {
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && strcmp(username, node->left->username) < 0)
    {
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(username, node->right->username) > 0)
    {
        return leftRotate(node);
    }

    if (balance > 1 && strcmp(username, node->left->username) > 0)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(username, node->right->username) < 0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

AVLNode *search(AVLNode *root, char *username)
{
    while (root != NULL)
    {
        if (strcmp(username, root->username) < 0)
        {
            root = root->left;
        }
        else if (strcmp(username, root->username) > 0)
        {
            root = root->right;
        }
        else
        {
            return root;
        }
    }
    return NULL;
}

void start()
{
    printf("\n");
}

User *login()
{
    char username[101];
    char password[101];
    static User user;

    printf("Enter username: ");
    scanf("%100s", username);
    printf("Enter password: ");
    scanf("%100s", password);

    char hashedpassword[MAX + sizeof(char)];
    hash(password, hashedpassword);

    AVLNode *node = search(root, username);
    if (node && strcmp(node->hashedpassword, hashedpassword) == 0)
    {
        user.username = malloc(strlen(node->username) + 1);
        strcpy(user.username, node->username);
        user.password = malloc(strlen(password) + 1);
        strcpy(user.password, password);
        user.hashedpassword = malloc(strlen(hashedpassword) + 1);
        strcpy(user.hashedpassword, hashedpassword);

        return &user;
    }

    printf("Invalid username or password.\n");
    return NULL;
}

User *signup()
{
    char username[101];
    char password[101];
    static User user;

    printf("Enter username: ");
    scanf("%100s", username);

    printf("Enter password: ");
    scanf("%100s", password);

    char hashedpassword[MAX + sizeof(char)];
    hash(password, hashedpassword);

    /* Check if users.txt exists*/
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        file = fopen("users.txt", "w");
        if (file == NULL)
        {
            printf("Error creating file.\n");
            return NULL;
        }
    }
    fclose(file); 
    /* Close the file if it was just created or already exists*/

    root = insert(root, username, hashedpassword);

    /* Add the new user to users.txt*/
    file = fopen("users.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return NULL;
    }
    fprintf(file, "%s:%s\n", username, hashedpassword);
    fclose(file);

    user.username = malloc(strlen(username) + 1);
    strcpy(user.username, username);
    user.password = malloc(strlen(password) + 1);
    strcpy(user.password, password);
    user.hashedpassword = malloc(strlen(hashedpassword) + 1);
    strcpy(user.hashedpassword, hashedpassword);

    printf("Signed up successfully!\n");
    return &user;
}


/* chagne password*/
void changePassword(User *user)
{
    FILE *file, *tempFile;
    char line[256]; /* Buffer for each line in the file */
    int found = 0;

    char currentPassword[101];
    char newPassword[101];

    printf("Enter your current password: ");
    scanf("%100s", currentPassword);

    char hashedpassword[MAX + sizeof(char)];
    hash(currentPassword, hashedpassword);

    if (strcmp(user->password, currentPassword) != 0)
    {
        printf("Incorrect password.\n");
        return;
    }

    printf("Enter your new password: ");
    scanf("%100s", newPassword);

    file = fopen("users.txt", "r");
    tempFile = fopen("temp.txt", "w");

    if (!file || !tempFile)
    {
        printf("Error opening file");
        return;
    }

    /* read file */
    while (fgets(line, sizeof(line), file))
    {
        char username[101];
        char password[101];

        /* Parse the username and password from the line */
        sscanf(line, "%100[^:]:%100s", username, password);

        /* Check if the current line matches the user's username */
        printf("%s %s\n", password, hashedpassword);
        if (strcmp(username, user->username) == 0 && strcmp(password, hashedpassword) == 0)
        {
            /* User found */
            hash(newPassword, hashedpassword);
            fprintf(tempFile, "%s:%s\n", username, hashedpassword);
            found = 1;
        }
        else
        {
            /* Write the original line to the temp file */
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    /* if user was found */
    if (found)
    {
        /* Replace the original file with the updated temp file */
        if (remove("users.txt") != 0 || rename("temp.txt", "users.txt") != 0)
        {
            printf("Error updating file");
            return;
        }
        /* Rebuild the AVL tree from the updated users.txt file*/
        rebuildAVLTreeFromFile();
        printf("Password changed successfully.\n");
    }
    else
    {
        remove("temp.txt"); /* Remove the temporary file */
        printf("User not found.\n");
    }
}

void rebuildAVLTreeFromFile()
{
    freeAVLTree(root);
    root = NULL;

    FILE *file = fopen("users.txt", "r");
    if (!file)
    {
        printf("Error opening file.\n");
        return;
    }

    char username[101];
    char hashedpassword[MAX + sizeof(char)];

   
    while (fscanf(file, "%100[^:]:%100s\n", username, hashedpassword) == 2)
    {
        root = insert(root, username, hashedpassword);
    }

    fclose(file);
}

void freeAVLTree(AVLNode *node)
{
    if (node == NULL)
        return;

    freeAVLTree(node->left);
    freeAVLTree(node->right);
    free(node);
}
