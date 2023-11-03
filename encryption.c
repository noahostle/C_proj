#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encryption.h"
#include "file_manager.h"

/*djb2*/
void hash(char* str, char* hashed) {
		unsigned long hash = 5381;

		int c;
		while ((c = *str++))
			hash = ((hash << 5) + hash) + c;

		sprintf(hashed, "%ld", hash); 

}


/*XOR cipher*/
void encrypt(char* ptext, char* key, int plen){
	/*int plen = strlen(ptext);*/
	/*some bytes after ciphering may = 0, which null terminates string,
	  so read in length before cipher to avoid early terminated strings.*/
	int klen = strlen(key);

	int i;
	for (i = 0; i < plen; i++) {
		/*XOR the plaintext with the password, when password*/
		/*runs out of characters, loop back to start.       */
		ptext[i] ^= key[i % klen];
	}

}
/*
Note: this cipher is cryptographically weak, since we are reusing keys.
*/

void encryptFile(User* user, char* path){
	FILE* file = fopen(path, "rb"); /* Open file in binary mode for reading */

    if (!file) {
        perror("Error opening source file.\n");
        return;
    }

	fseek(file, 0, SEEK_END);
	unsigned int length = ftell(file);
	char* buffer = malloc(length);
	fseek(file, 0, SEEK_SET);

	if (buffer){
		fread(buffer, 1, length, file);
	}

	remove(path);

	int buflen = strlen(buffer);

	encrypt(buffer, user->password, buflen);


	/* remove .rle */
    path[strlen(path)-4] = 0;


    
    FILE* newfile = fopen(path, "w");

    if (!newfile) {
        perror("Error creating encrypted file.\n");
        return;
    }

    fwrite(buffer, sizeof(char), buflen ,newfile);

    fclose(newfile);
    fclose(file);
    free(buffer);

}


void decryptFile(User* user, char* path){



	FILE* file = fopen(path, "rb"); /* Open file in binary mode for reading */

    if (!file) {
        perror("Error opening source file.\n");
        return;
    }

	fseek(file, 0, SEEK_END);
	unsigned int length = ftell(file);
	char* buffer = malloc(length);
	fseek(file, 0, SEEK_SET);

	if (buffer){
		fread(buffer, 1, length, file);
	}

	int buflen = length;

	decrypt(buffer, user->password, buflen);

	strcat(path,".rle");

    FILE* newfile = fopen(path, "wb");

    if (!newfile) {
        perror("Error creating decrypted file.\n");
        return;
    }


    fwrite(buffer, sizeof(char), buflen ,newfile);





	fclose(newfile);
    fclose(file);
    free(buffer);

}

void decrypt(char* ptext, char* key, int plen){
	encrypt(ptext, key, plen);
}
/*
Note: running encrypt again on already encrypted data is equivalent to decrypting
*/




void poc(){


	char hashed[MAX + sizeof(char)]; 
	char hi[]="aaaaaa";
	hash(hi,hashed);
	printf("%s",hashed);




	char ptext[] = "Hash this string please";
	char pass[] = "Password123";

	int plen = strlen(ptext);

	printf("Plaintext: %s\n\n",ptext);
	encrypt(ptext, pass, plen);


	printf("Encrypted (byte values): ");
	int x;
	for (x=0; x<plen; x++){
		printf("%d ",ptext[x]);
	}
	printf("\n\n");


	decrypt(ptext, pass, plen);
	printf("Decrypted: %s\n",ptext);
}
