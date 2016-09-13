all: sshell.c
	gcc sshell.c -o sshell

debug: sshell.c
	gcc -DDEBUG -g sshell.c -o debug-sshell
