#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>


#define RESERVED_SIZE 8192

typedef struct
{
    size_t size;
} mem_header_t;

/*****************************************************************************/
/* Hooking glibc function poninters                                          */
/*****************************************************************************/
static void *(*real_calloc) (size_t, size_t) = NULL;
static void *(*real_malloc) (size_t)         = NULL;
static void *(*real_realloc)(void *, size_t) = NULL;
static void  (*real_free)   (void *)         = NULL;

/*****************************************************************************/
/* The keyword '__thread': is used to define thread-local variables.         */
/* This helps to avoid infinite recursive call                               */
/*****************************************************************************/
static __thread int no_hook;

static uint8_t  reserved_memory[RESERVED_SIZE] = { 0 };
static uint64_t allocated_bytes;

/*****************************************************************************/
/* __attribute__((constructor)) syntax:                                      */
/* This particular GCC syntax, when used with a function,                    */
/* executes the same function at the startup of the program,                 */
/* i.e before main() function.                                               */
/*****************************************************************************/
static void __attribute__((constructor)) init(void)
{
    real_calloc  = (void *(*) (size_t, size_t)) dlsym(RTLD_NEXT, "calloc");
    real_malloc  = (void *(*) (size_t))         dlsym(RTLD_NEXT, "malloc");
    real_realloc = (void *(*) (void *, size_t)) dlsym(RTLD_NEXT, "realloc");
    real_free    = (void  (*) (void *))         dlsym(RTLD_NEXT, "free");

    no_hook = 0;
    allocated_bytes = 0;

    fprintf(stderr, "ADDRESS \tNAME    SIZE\tPOINTER  \tHEAP\n");
    fprintf(stderr, "------- \t----    ----\t-------  \t0\n");
}

/*****************************************************************************/
/* __attribute__((destructor)) syntax:                                       */
/* This particular GCC syntax, when used with a function,                    */
/* executes the same function at the end of the program,                     */
/* i.e before exit() function.                                               */
/*****************************************************************************/
static void __attribute__((destructor)) deinit(void)
{
    fprintf(stderr, "------- \t----    ----\t-------  \t%zu\n", allocated_bytes);
}

static void print_log(const char  *func,
                      const void  *caller,
                      const void  *addr,
                      const size_t size)
{
    fprintf(stderr, "%p\t%s\t%zu\t%p\t%zu\n",
        caller, func, size, addr, allocated_bytes);
}

/*****************************************************************************/
/* In multi-thread mode, dlsym() uses calloc() at the first time.            */
/* - calloc() requires dlsym()                                               */
/* - dlsym() requires calloc() … !!                                          */
/* Set up special calloc() for the firs call of calloc().                    */
/*****************************************************************************/
static void *get_reserved_memory(size_t nmemb, size_t size)
{
    if ((nmemb * size) > RESERVED_SIZE)
    {
        fprintf(stderr, "<%s> Can't provide stack reserved memory\n", __FUNCTION__);
        assert(0);
    }

    return reserved_memory;
}

static void *increase_heap_usage(void *ptr, const size_t size, const bool is_realloc)
{
    if (!ptr)
    {
        fprintf(stderr, "<%s> Can't get memory pointer\n", __FUNCTION__);
        assert(0);
    }

    if (is_realloc)
    {
        size_t old_size = ((mem_header_t *)(ptr))->size;
        ((mem_header_t *)(ptr))->size = size;
        allocated_bytes -= old_size;
        allocated_bytes += size;
    }
    else
    {
        ((mem_header_t *)(ptr))->size = size;
        allocated_bytes += size;
    }

    return ptr + sizeof(mem_header_t);
}

static void *decrease_heap_usage(void *ptr)
{
    mem_header_t *mem_header = (mem_header_t *)(ptr - sizeof(mem_header_t));
    if (!mem_header)
    {
        fprintf(stderr, "<%s> Can't get pointer size\n", __FUNCTION__);
        assert(0);
    }

    allocated_bytes -= mem_header->size;

    return ptr - sizeof(mem_header_t);
}

static size_t get_heap_usage_by_ptr(void *ptr)
{
    return ((mem_header_t *)(ptr - sizeof(mem_header_t)))->size;
}

/*****************************************************************************/

void *malloc(size_t size)
{
    if (no_hook)
        return (*real_malloc)(size);

    no_hook = 1;

    void *ret_ptr = (*real_malloc)(size + sizeof(mem_header_t));
    if (ret_ptr)
    {
        ret_ptr = increase_heap_usage(ret_ptr, size, false);
    }

    print_log(__FUNCTION__, __builtin_return_address(0), ret_ptr, size);

    no_hook = 0;

    return ret_ptr;
}

void *calloc(size_t nmemb, size_t size)
{
    if (real_calloc == NULL)
        return get_reserved_memory(nmemb, size);

    if (no_hook)
        return (*real_calloc)(nmemb, size);

    no_hook = 1;

    // Replace calloc to malloc() + memset()
    void *ret_ptr = (*real_malloc)((nmemb * size) + sizeof(mem_header_t));
    if (ret_ptr)
    {
        memset(ret_ptr, 0, (nmemb * size) + sizeof(mem_header_t));
        ret_ptr = increase_heap_usage(ret_ptr, (nmemb * size), false);
    }

    print_log(__FUNCTION__, __builtin_return_address(0), ret_ptr, size);

    no_hook = 0;

    return ret_ptr;
}

void *realloc(void *ptr, size_t size)
{
    if (no_hook)
        return (*real_realloc)(ptr, size);

    no_hook = 1;

    bool is_realloc = false;
    if (ptr)
    {
        is_realloc = true;
        ptr = ptr - sizeof(mem_header_t);
    }

    void *ret_ptr = (*real_realloc)(ptr, (size + sizeof(mem_header_t)));
    if (ret_ptr)
    {
        ret_ptr = increase_heap_usage(ret_ptr, size, is_realloc);
    }

    print_log(__FUNCTION__, __builtin_return_address(0), ret_ptr, size);


    no_hook = 0;

    return ret_ptr;
}

void free(void *ptr)
{
    if (!ptr)
        return;

    print_log(__FUNCTION__, __builtin_return_address(0), ptr, get_heap_usage_by_ptr(ptr));

    ptr = decrease_heap_usage(ptr);

    (*real_free)(ptr);
}
