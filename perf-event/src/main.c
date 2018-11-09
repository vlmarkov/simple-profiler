#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include <profiler-hw-events.h>
#include <profiler-sampling.h>


/*****************************************************************************/
/* This funtion used to:                                                     */
/* - print help message                                                      */
/*****************************************************************************/
static void usage(char const *argv0)
{
    fprintf(stderr, "usage: %s <your-test-program>\n", argv0);
}

/*****************************************************************************/
/* This funtion used to:                                                     */
/* - check sudo rights                                                       */
/*****************************************************************************/
static void sudo_check(char const *argv0)
{
    if (getuid() != 0 && geteuid() != 0)
    {
        fprintf(stderr, "usage: sudo %s <your-test-program>\n", argv0);
        exit(-1);
    }
}

int main(int argc, char **argv)
{
    sudo_check(argv[0]);

    if (argc < 2)
    {
        usage(argv[0]);
    }
    else
    {
#ifdef SAMPLING
        profiler_sampling_run(argc, argv);
#else
        profiler_events_run(argc, argv);
#endif /* SAMPLING */
    }

    return 0;
}

