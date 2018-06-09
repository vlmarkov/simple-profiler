all: main

main: main.c
	gcc -g -Wall -O2 -o main main.c -lpthread

clean:
	rm -f main
