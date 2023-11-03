CC = gcc -Wall -Werror -ansi

compile: encryption.o file_manager.o main.o compression.o login.o
	$(CC) -o cloudhosting encryption.o file_manager.o main.o compression.o login.o
	rm file_manager.o encryption.o main.o compression.o login.o

main.o:
	$(CC) -c -o main.o main.c

encryption.o:
	$(CC) -c -o encryption.o encryption.c

file_manager.o: file_manager.h
	$(CC) -c -o file_manager.o file_manager.c

compression.o: compression.h
	$(CC) -c -o compression.o compression.c

login.o: login.h
	$(CC) -c -o login.o login.c

clean:
	rm file_manager.o encryption.o main.o compression.o login.o cloudhosting
