all: sshell.c
	gcc sshell.c -o sshell -std=c99

debug: sshell.c
	gcc -DDEBUG -g sshell.c -o debug-sshell -std=c99
