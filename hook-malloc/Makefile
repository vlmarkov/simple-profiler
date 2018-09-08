all: sample hook_malloc

sample:
	mkdir -p build
	gcc -g -Wall -O2 -o build/sample samples/sample.c -std=c11
	gcc -g -Wall -O2 -o build/leak-sample samples/leak-sample.c -std=c11

hook_malloc:
	mkdir -p build
	gcc -Wall src/hook_malloc.c -fPIC -shared -o build/lib_hook_malloc.so -ldl -rdynamic -pthread -std=c11

clean:
	rm -Rf build
