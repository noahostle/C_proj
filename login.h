#ifndef LOGIN_H
#define LOGIN_H

/* Represents a user in the system */
typedef struct
{
    char *username;
    char *password;
    char *hashedpassword;
} User;

typedef struct AVLNode AVLNode;

/* Function declarations */
void start();
User *login();
User *signup();
void changePassword(User *user);
void rebuildAVLTreeFromFile();
void freeAVLTree(AVLNode *node);

#endif