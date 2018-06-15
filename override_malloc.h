#ifndef _OVERRIDE_MALLOC_H_
#define _OVERRIDE_MALLOC_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>

typedef struct
{
    long size;
} header_t;

static long long alloced_bytes = 0;

static inline void free_(void       *ptr,
                         const char *file,
                         const char *fun,
                         const int   ln)
{
    void     *p = NULL;
    header_t *h = NULL;

    if (!ptr)
    {
        fprintf(stderr, "<%s> Not valid pointer, passed from: %s %s %d\n",
            __FUNCTION__, file, fun, ln);
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
    printf("<%s   >\t%s\t%s()\t%d\tptr[%p]\tsize(%ld)\tpid = %ld/%ld\n",
           __FUNCTION__, file, fun, ln, p, h->size,
           (long)getpid(), syscall(SYS_gettid));

    // Actual function call
    free(p);
}

static inline void *malloc_(size_t      size,
                            const char *file,
                            const char *fun,
                            const int   ln)
{
    void     *p = NULL;
    header_t *h = NULL;

    // Actual function call
    p = malloc(size + sizeof(header_t));
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
    printf("<%s >\t%s\t%s()\t%d\tptr[%p]\tsize(%ld)\tpid = %ld/%ld\n",
           __FUNCTION__, file, fun, ln, ptr, h->size,
           (long)getpid(), syscall(SYS_gettid));

    return ptr;
}

static inline void *calloc_(size_t      nmemb,
                            size_t      size,
                            const char *file,
                            const char *fun,
                            const int   ln)
{
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
    printf("<%s >\t%s\t%s()\t%d\tptr[%p]\tsize(%ld)\tpid = %ld/%ld\n",
           __FUNCTION__, file, fun, ln, ptr, h->size,
           (long)getpid(), syscall(SYS_gettid));

    return ptr;
}

static inline void *realloc_(void       *ptr,
                             size_t      size,
                             const char *file,
                             const char *fun,
                             const int   ln)
{
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
    p = realloc(p, size);
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
    printf("<%s>\t%s\t%s()\t%d\tptr[%p]\tsize(%ld)\tpid = %ld/%ld\n",
           __FUNCTION__, file, fun, ln, realloced_ptr, h->size,
           (long)getpid(), syscall(SYS_gettid));

    return realloced_ptr;
}

static inline void show_heap_usage(void)
{
    // Replace by your logging message
    printf("<%s> Still in heap %lld bytes\n",
           __FUNCTION__,  alloced_bytes);
}

#define free(ptr)           free_(ptr, __FILE__, __FUNCTION__, __LINE__)
#define malloc(size)        malloc_(size, __FILE__, __FUNCTION__, __LINE__)
#define calloc(nmemb, size) calloc_(nmemb, size, __FILE__, __FUNCTION__, __LINE__)
#define realloc(ptr, size)  realloc_(ptr, size, __FILE__, __FUNCTION__, __LINE__)
#define show_heap_usage()   show_heap_usage()

#endif /* _OVERRIDE_MALLOC_H_ */
