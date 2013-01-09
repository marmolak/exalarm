all:
	gcc -o main main.c exalarm.c -Wall -Wextra
debug:
	gcc -DDEBUG -o main main.c exalarm.c -Wall -Wextra
