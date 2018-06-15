all: main_static main_dynamic lib_alloc

main_static: main.c
	gcc -g -Wall -O2 -o main_static main.c -D STATIC

main_dynamic: main.c
	gcc -g -Wall -O2 -o main_dynamic main.c

lib_alloc: lib_alloc.c
	gcc -Wall lib_alloc.c -fPIC -shared -o lib_alloc.so -ldl

clean:
	rm -f main_* *.so
