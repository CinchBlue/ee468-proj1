all: sshell.c sshell2.c
	gcc sshell.c -o sshell
	gcc sshell2.c -o sshell2

debug: sshell.c sshell2.c
	gcc -DDEBUG -g sshell.c -o debug-sshell
	gcc -DDEBUG -g sshell2.c -o debug-sshell2
