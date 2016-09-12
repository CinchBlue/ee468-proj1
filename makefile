all: sshell.c
	gcc sshell.c -o sshell

debug: sshell.c
	gcc -DDEBUG sshell.c -o debug-sshell
