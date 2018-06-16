#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <execinfo.h>


void rollback_trace(int sig)
{
    void *buffer[128];

    int size = backtrace(buffer, 128);

    char **strings = backtrace_symbols(buffer, size);
    if (!strings)
    {
        fprintf(stderr, "Can't get backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    printf("\n=== BACKTRACE START ===\n");
    for (int i = 0; i < size; i++)
    {
        printf("%s\n", strings[i]);
    }
    printf("=== BACKTRACE END  ===\n");
    free(strings);
}

void test_func2(void)
{
    char *calloc_ptr = (char *)calloc(1, sizeof(char) * 222);
    if (!calloc_ptr)
    {
        fprintf(stderr, "Can't allocate memory, by calloc()\n");
        return;
    }

    free(calloc_ptr);
}

void test_func1(void)
{
    test_func2();
}

int main(int argc, char const *argv[])
{
    if (signal(SIGUSR2, rollback_trace) == SIG_ERR)
    {
        fprintf(stderr, "Can't init signal handler\n");
        return -1;
    }

    test_func1();

    return 0;
}
