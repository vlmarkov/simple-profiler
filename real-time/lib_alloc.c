#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

/*
 * There is no recursive protection, it will segfault
 */

typedef struct
{
    long size;
} header_t;

static void  (*real_free)(void *ptr)                   = NULL;
static void* (*real_malloc)(size_t size)               = NULL;
static void* (*real_calloc)(size_t nmemb, size_t size) = NULL;
static void* (*real_realloc)(void *ptr, size_t size)   = NULL;

static long long alloced_bytes;


static void init(void)
{
    real_malloc  = dlsym(RTLD_NEXT, "malloc");
    real_calloc  = dlsym(RTLD_NEXT, "calloc");
    real_realloc = dlsym(RTLD_NEXT, "realloc");
    real_free    = dlsym(RTLD_NEXT, "free");

    alloced_bytes = 0;
}

void *malloc(size_t size)
{
    if (!real_malloc)
        init();

    void     *p = NULL;
    header_t *h = NULL;

    // Actual function call
    p = real_malloc(size + sizeof(header_t));
    if (!p)
    {
        return NULL;
    }

    h = (header_t *)p;
    if (!h)
    {
        fprintf(stderr, "<%s> Can't get pointer size\n", __FUNCTION__);
        abort();
    }

    h->size = size;

    alloced_bytes += h->size;

    void *ptr = p + sizeof(header_t);
    if (!ptr)
    {
        fprintf(stderr, "<%s> Can't get pointer\n", __FUNCTION__);
        abort();
    }

    // Replace by your logging message
    printf("<%s >\tptr[%p]\tsize(%ld)\tpid = %ld/%ld\n",
           __FUNCTION__, ptr, h->size, (long)getpid(), syscall(SYS_gettid));

    return ptr;
}

void *calloc(size_t nmemb, size_t size)
{
    if (!real_calloc)
        init();

    void     *p = NULL;
    header_t *h = NULL;

    // Replace calloc to malloc() + memset()
    p = malloc((nmemb * size) + sizeof(header_t));
    if (!p)
    {
        return NULL;
    }
    memset(p, 0, (nmemb * size) + sizeof(header_t));

    h = (header_t *)p;
    if (!h)
    {
        fprintf(stderr, "<%s> Can't get pointer size\n", __FUNCTION__);
        abort();
    }

    h->size = (nmemb * size);

    alloced_bytes += h->size;

    void *ptr = p + sizeof(header_t);
    if (!ptr)
    {
        fprintf(stderr, "<%s> Can't get pointer\n", __FUNCTION__);
        abort();
    }

    // Replace by your logging message
    printf("<%s >\tptr[%p]\tsize(%ld)\tpid = %ld/%ld\n",
           __FUNCTION__, ptr, h->size, (long)getpid(), syscall(SYS_gettid));

    return ptr;
}

void *realloc(void *ptr, size_t size)
{
    if (!real_realloc)
        init();

    void     *p = NULL;
    header_t *h = NULL;

    void *realloced_ptr = NULL;

    int new_pointer = 0;

    if (ptr)
    {
        // We try to realloc allocated pointer
        p = ptr - sizeof(header_t);
        if (!p)
        {
            fprintf(stderr, "<%s> Can't get pointer\n", __FUNCTION__);
            abort();
        }
    }
    else
    {
        // If ptr is NULL, then the call is equivalent to malloc(size)
        p = ptr;
        new_pointer = 1;

        if (size != 0)
        {
            size += sizeof(header_t);
        }
    }

    /*
     * If size is equal to zero, and ptr is not NULL,
     * then the call is equivalent to free(ptr).
     */
    if (ptr && size == 0)
        fprintf(stderr, "<%s> Try to realloc to zero size\n", __FUNCTION__);

    // Actual function call
    p = real_realloc(p, size);
    if (!p)
    {
        return p;
    }

    if (size != 0)
    {
        h = (header_t *)p;
        if (!h)
        {
            fprintf(stderr, "<%s> Can't get pointer size\n", __FUNCTION__);
            abort();
        }

        if (new_pointer)
        {
            h->size = size - sizeof(header_t);
            alloced_bytes += h->size;
        }
        else
        {
            h->size       += size;
            alloced_bytes += size;
        }

        realloced_ptr = p + sizeof(header_t);
    }

    // Replace by your logging message
    printf("<%s>\tptr[%p]\tsize(%ld)\tpid = %ld/%ld\n",
           __FUNCTION__, realloced_ptr, h->size, (long)getpid(), syscall(SYS_gettid));

    return realloced_ptr;
}

void free(void *ptr)
{
    if (!real_free)
        init();

    void     *p = NULL;
    header_t *h = NULL;

    if (!ptr)
    {
        return;
    }

    p = ptr - sizeof(header_t);
    if (!p)
    {
        fprintf(stderr, "<%s> Can't get pointer\n", __FUNCTION__);
        abort();
    }

    h = (header_t *)p;
    if (!h)
    {
        fprintf(stderr, "<%s> Can't get pointer size\n", __FUNCTION__);
        abort();
    }

    alloced_bytes -= h->size;

    // Replace by your logging message
    printf("<%s   >\tptr[%p]\tsize(%ld)\tpid = %ld/%ld\n",
           __FUNCTION__, p, h->size, (long)getpid(), syscall(SYS_gettid));

    // Actual function call
    real_free(ptr);
}
