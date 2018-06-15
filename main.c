#include <stdio.h>
#include <stdlib.h>

#ifdef STATIC
#include "override_malloc.h"
#endif /* STATIC */

int main(int argc, char const *argv[])
{
    char *malloc_ptr = (char *)malloc(sizeof(char) * 111);
    if (!malloc_ptr)
    {
        fprintf(stderr, "Can't allocate memory, by malloc()\n");
        return -1;
    }

    char *calloc_ptr = (char *)calloc(1, sizeof(char) * 222);
    if (!calloc_ptr)
    {
        fprintf(stderr, "Can't allocate memory, by calloc()\n");
        return -1;
    }

    char *realloc_ptr = (char *)realloc(NULL, sizeof(char) * 333);
    if (!realloc_ptr)
    {
        fprintf(stderr, "Can't allocate memory, by calloc()\n");
        return -1;
    }

    realloc_ptr = (char *)realloc(realloc_ptr, sizeof(char) * 444);
    if (!realloc_ptr)
    {
        fprintf(stderr, "Can't allocate memory, by calloc()\n");
        return -1;
    }

    free(realloc_ptr);
    free(calloc_ptr);
    free(malloc_ptr);


#ifdef STATIC
    show_heap_usage();
#endif /* STATIC */

    return 0;
}
