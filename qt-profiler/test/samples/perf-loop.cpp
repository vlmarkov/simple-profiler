#include <unistd.h>

void a(void)
{
    long long sum1 = 0;

    for (long long i = 0; i < 1E5; i++)
        sum1 += i;
}

void b(void)
{
    long long sum1 = 0;

    for (long long i = 0; i < 1E7; i++)
        sum1 += i;
}

void c(void)
{
    long long sum1 = 0;

    for (long long i = 0; i < 1E9; i++)
        sum1 += i;
}

int main(void)
{
    a();
    sleep(1);

    b();
    sleep(1);

    c();

    return 0;
}
