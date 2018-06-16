all: main lib_alloc

main: main.c
	gcc -g -Wall -O2 -o main main.c

lib_alloc: lib_alloc.c
	gcc -Wall lib_alloc.c -fPIC -shared -o lib_alloc.so -ldl

clean:
	rm -f main *.so
