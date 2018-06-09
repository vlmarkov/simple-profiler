#include <stdio.h>

#include "override_malloc.h"

int main(int argc, char const *argv[])
{
    char *malloc_ptr = (char *)malloc(sizeof(char) * 256);
    if (!malloc_ptr)
    {
        fprintf(stderr, "Can't allocate memory, by malloc()\n");
        return -1;
    }

    char *calloc_ptr = (char *)calloc(1, sizeof(char) * 256);
    if (!calloc_ptr)
    {
        fprintf(stderr, "Can't allocate memory, by calloc()\n");
        return -1;
    }


    char *realloc_ptr = (char *)realloc(NULL, sizeof(char) * 256);
    if (!realloc_ptr)
    {
        fprintf(stderr, "Can't allocate memory, by calloc()\n");
        return -1;
    }

    free(realloc_ptr);
    free(calloc_ptr);
    //free(malloc_ptr);
    
    return 0;
}
