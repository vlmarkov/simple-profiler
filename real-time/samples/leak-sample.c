#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char *malloc_ptr = (char *)malloc(sizeof(char) * 111);
    if (!malloc_ptr)
    {
        fprintf(stderr, "Can't allocate memory, by malloc()\n");
        return -1;
    }

    //free(malloc_ptr);

    return 0;
}
