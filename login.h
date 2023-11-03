#ifndef LOGIN_H
#define LOGIN_H

/* Represents a user in the system */
typedef struct
{
    char *username;
    char *password;
    char *hashedpassword;
} User;

/* Function declarations */
void start();
User *login();
User *signup();
void changePassword(User *user);

#endif
