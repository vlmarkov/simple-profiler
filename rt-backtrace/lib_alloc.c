#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/syscall.h>

/*
 * There is no recursive protection
 */

static void  (*real_free)(void *ptr)                   = NULL;
static void* (*real_malloc)(size_t size)               = NULL;
static void* (*real_calloc)(size_t nmemb, size_t size) = NULL;
static void* (*real_realloc)(void *ptr, size_t size)   = NULL;

static void init(void)
{
    real_malloc  = dlsym(RTLD_NEXT, "malloc");
    real_calloc  = dlsym(RTLD_NEXT, "calloc");
    real_realloc = dlsym(RTLD_NEXT, "realloc");
    real_free    = dlsym(RTLD_NEXT, "free");
}

void *calloc(size_t nmemb, size_t size)
{
    if (!real_calloc)
        init();

    // Replace calloc to malloc() + memset()
    void *p = malloc((nmemb * size));
    if (!p)
    {
        return NULL;
    }
    memset(p, 0, (nmemb * size));

    // Replace by your logging message
    printf("<%s >\tptr[%p]\tsize(%ld)\tpid = %ld/%ld\n",
           __FUNCTION__, p, (nmemb * size), (long)getpid(), syscall(SYS_gettid));

    raise(SIGUSR2);

    return p;
}