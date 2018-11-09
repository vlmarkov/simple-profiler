#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <perf-event.h>


/*****************************************************************************/
/* Local prototypes                                                          */
/*****************************************************************************/
static void fork_and_exec_fail(const char *error);
static void fork_and_exec(int argc, char **argv, size_t hw_cnt,
                          uint32_t *hw, uint64_t *hw_id, uint64_t *hw_val);
static void exec_child(int argc, char **argv);
static void exec_parent(pid_t child_pid, size_t hw_cnt, uint32_t *hw,
                        uint64_t *hw_id, uint64_t  *hw_val);


/*****************************************************************************/
/* This funtion used to:                                                     */
/* - print error message                                                     */
/* - abort execution                                                         */
/*****************************************************************************/
static void fork_and_exec_fail(const char *error)
{
    fprintf(stderr, "%s\n", error);
    exit(EXIT_FAILURE);
}

/*****************************************************************************/
/* This funtion used to:                                                     */
/* - execute child testing program                                           */
/*****************************************************************************/
static void exec_child(int argc, char **argv)
{
    char *args[] = { argv[1], NULL };
    execve(argv[1], args, NULL);

    // Never reach
    _exit(EXIT_SUCCESS);
}

/*****************************************************************************/
/* This funtion used to:                                                     */
/* - monitor child                                                           */
/* - init perf-event                                                         */
/*****************************************************************************/
static void exec_parent(
    pid_t     child_pid,
    size_t    hw_cnt,
    uint32_t *hw,
    uint64_t       *hw_id,
    uint64_t       *hw_val)
{
    int fd = perf_events_init(child_pid, hw_cnt, hw, hw_id);
    if (fd < 0)
        fork_and_exec_fail("Can't perf events init");

    if (perf_event_start(fd, true) != 0)
        fork_and_exec_fail("Can't perf event start");

    int status = 0;

    while (true)
    {
        pid_t wait_pid = waitpid(child_pid, &status, WNOHANG | WUNTRACED | WCONTINUED);

        if (wait_pid == -1)
        {
            fork_and_exec_fail("waitpid(), error");
        }

        if (wait_pid == 0)
        {
            // Child still running
        }
        else if (wait_pid == child_pid)
        {
            // Child ended
            if (WIFEXITED(status))
            {
                fprintf(stderr, "<%s> Child %d ended normally\n",
                    __FUNCTION__, child_pid);
            }
            else if (WIFSIGNALED(status))
            {
                fprintf(stderr, "<%s> Child %d ended because of an uncaught signal\n",
                    __FUNCTION__, child_pid);
            }
            else if (WIFSTOPPED(status))
            {
                fprintf(stderr, "<%s> Child %d has stopped\n",
                    __FUNCTION__, child_pid);
            }
            break;
        }
    }

    if (perf_event_stop(fd, true) != 0)
        fork_and_exec_fail("Can't perf event stop");

    if (perf_events_read(fd, hw_id, hw_val) != 0)
        fork_and_exec_fail("Can't perf event read");
}

/*****************************************************************************/
/* This funtion used to:                                                     */
/* - fork and exec child                                                     */
/*****************************************************************************/
static void fork_and_exec(int argc, char **argv, size_t hw_cnt, uint32_t *hw,
                          uint64_t *hw_id, uint64_t *hw_val)
{
    pid_t child_pid = 0;

    switch (child_pid = fork())
    {
        case -1:
            fork_and_exec_fail("fork() failed");
            break;

        case 0:
            exec_child(argc, argv);
            break;

        default: 
            exec_parent(child_pid, hw_cnt, hw, hw_id, hw_val);
            break;
    }
}

/*****************************************************************************/
/* This funtion used to:                                                     */
/* - run perf-event sampling profiller                                       */
/*****************************************************************************/
void profiler_events_run(int argc, char **argv)
{
    uint32_t hw_cpu[] = {
        PERF_COUNT_HW_CPU_CYCLES,
        PERF_COUNT_HW_INSTRUCTIONS
    };

    uint32_t hw_cache[] = {
        PERF_COUNT_HW_CACHE_REFERENCES,
        PERF_COUNT_HW_CACHE_MISSES
    };

    uint32_t hw_branch[] = {
        PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
        PERF_COUNT_HW_BRANCH_MISSES
    };

    size_t hw_cpu_cnt    = sizeof(hw_cpu)   / sizeof(hw_cpu[0]);
    size_t hw_cache_cnt  = sizeof(hw_cache) / sizeof(hw_cache[0]);
    size_t hw_branch_cnt = sizeof(hw_branch) / sizeof(hw_branch[0]);

    uint64_t hw_id_cpu[hw_cpu_cnt];
    uint64_t hw_id_cache[hw_cache_cnt];
    uint64_t hw_id_branch[hw_branch_cnt];

    memset(hw_id_cpu,    0, sizeof(hw_id_cpu));
    memset(hw_id_cache,  0, sizeof(hw_id_cache));
    memset(hw_id_branch, 0, sizeof(hw_id_branch));

    uint64_t hw_val_cpu[hw_cpu_cnt];
    uint64_t hw_val_cache[hw_cache_cnt];
    uint64_t hw_val_branch[hw_branch_cnt];

    fork_and_exec(argc, argv, hw_cpu_cnt,    hw_cpu,    hw_id_cpu,    hw_val_cpu);
    fork_and_exec(argc, argv, hw_cache_cnt,  hw_cache,  hw_id_cache,  hw_val_cache);
    fork_and_exec(argc, argv, hw_branch_cnt, hw_branch, hw_id_branch, hw_val_branch);

    uint64_t vals[hw_cpu_cnt + hw_cache_cnt + hw_branch_cnt];
    vals[0] = hw_val_cpu[0];
    vals[1] = hw_val_cpu[1];
    vals[2] = hw_val_cache[0];
    vals[3] = hw_val_cache[1];
    vals[4] = hw_val_branch[0];
    vals[5] = hw_val_branch[1];

    perf_events_hw_show(vals);
}
