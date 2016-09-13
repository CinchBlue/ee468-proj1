all: sshell.c sshell2.c
	gcc sshell.c -o sshell -std=c99
	gcc sshell2.c -o sshell2 -std=c99

debug: sshell.c sshell2.c
	gcc -DDEBUG -g sshell.c -o debug-sshell -std=c99
	gcc -DDEBUG -g sshell2.c -o debug-sshell2 -std=c99
