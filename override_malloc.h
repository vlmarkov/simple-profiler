#ifndef _OVERRIDE_MALLOC_H_
#define _OVERRIDE_MALLOC_H_

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>


static inline void wrap_free(void       *ptr,
                             const char *file,
                             const char *fun,
                             const int   ln)
{
    // Replace by your logging message
    printf("<%s>\t%s\t%s()\t%d\tptr[%p]\tpid = %ld/%ld\n",
           __FUNCTION__, file, fun, ln, ptr, (long)getpid(), syscall(SYS_gettid));

    // Actual function call
    free(ptr); 

}

static inline void *wrap_malloc(size_t      size,
                                const char *file,
                                const char *fun,
                                const int   ln)
{
    // Actual function call
    void *p = malloc(size);

    // Replace by your logging message
    printf("<%s>\t%s\t%s()\t%d\tptr[%p]\tpid = %ld/%ld\n",
           __FUNCTION__, file, fun, ln, p, (long)getpid(), syscall(SYS_gettid));

    return p;
}

static inline void *wrap_calloc(size_t      nmemb,
                                size_t      size,
                                const char *file,
                                const char *fun,
                                const int   ln)
{
    // Actual function call
    void *p = calloc(nmemb, size);

    // Replace by your logging message
    printf("<%s>\t%s\t%s()\t%d\tptr[%p]\tpid = %ld/%ld\n",
           __FUNCTION__, file, fun, ln, p, (long)getpid(), syscall(SYS_gettid));

    return p;
}

static inline void *wrap_realloc(void       *ptr,
                                 size_t      size,
                                 const char *file,
                                 const char *fun,
                                 const int   ln)
{
    // Actual function call
    void *p = realloc(ptr, size);

    // Replace by your logging message
    printf("<%s>\t%s\t%s()\t%d\tptr[%p]\tpid = %ld/%ld\n",
           __FUNCTION__, file, fun, ln, p, (long)getpid(), syscall(SYS_gettid));

    return p;
}

#define free(ptr)           wrap_free(ptr, __FILE__, __FUNCTION__, __LINE__)
#define malloc(size)        wrap_malloc(size, __FILE__, __FUNCTION__, __LINE__)
#define calloc(nmemb, size) wrap_calloc(nmemb, size, __FILE__, __FUNCTION__, __LINE__)
#define realloc(ptr, size)  wrap_realloc(ptr, size, __FILE__, __FUNCTION__, __LINE__)

#endif /* _OVERRIDE_MALLOC_H_ */
