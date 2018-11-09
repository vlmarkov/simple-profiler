#pragma once

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <signal.h>
#include <stddef.h>


#define SAMPLE_PERIOD 100000 // Time between samples

typedef void (*cb_print_t)(void *buffer);

typedef struct perf_event_attr      perf_t;
typedef struct perf_event_mmap_page perf_page_t;
typedef struct perf_event_header    perf_header_t;


typedef struct read_format
{
    uint64_t nr;
    struct {
        uint64_t value, id;
    } values[];
} read_format_t;

typedef struct
{
    perf_header_t header;
    uint64_t      ip;
    uint32_t      pid, tid;
} perf_record_sample_t;


/*****************************************************************************/
/* Perf-event: common API                                                    */
/*****************************************************************************/
int perf_event_open(perf_t *pe, pid_t pid, int cpu, int g_fd, unsigned long flags);
int perf_event_close(int fd, void *mmapped);
int perf_event_start(int fd, bool is_group);
int perf_event_stop(int fd, bool is_group);
int perf_event_mmap(perf_t *pe, int fd, void **mmapped);


/*****************************************************************************/
/* Perf-event: event API                                                     */
/*****************************************************************************/
int perf_event_init_instructions(perf_t *pe);
int perf_event_read_instructions(int fd, long long *inst);

int perf_events_init(pid_t pid, size_t size, uint32_t *events, uint64_t *ids);
int perf_events_read(int fd, uint64_t *ids, uint64_t *vals);
void perf_events_hw_show(uint64_t *hw_vals);


/*****************************************************************************/
/* Perf-event: sampling API                                                  */
/*****************************************************************************/
int perf_event_init_sampling(perf_t *pe);
void perf_event_ring_buffer_copy_get(perf_page_t *page, size_t bytes, void *dest);
void perf_event_ring_buffer_page_print(perf_page_t *page);
void perf_event_ring_buffer_data_print(perf_page_t *page, size_t size, cb_print_t cb);
void perf_event_sample_print(void *buffer);
