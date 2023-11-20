#define _GNU_SOURCE
#include <sched.h>
#include <stdint.h>
#include <stddef.h>

#include "thread.h"

struct thread{
    enum THREAD_STATUS status;
    uint16_t id_thread;
    uint16_t thread_timeout;
    void *(*thread_fun)(void *);
};

static uint64_t thrad_id=0;

//public
uint32_t new_thread(void *(*thread_fun)(void *), uint16_t timeout)
{

    return 0;
}

void thread_start(uint32_t thread_id);

void thread_wait(uint32_t thread_id);

void thread_suspend(uint32_t thread_id);

void thread_resume(uint32_t thread_id);

void thread_kill(uint32_t thread_id);

THREAD_STATUS thread_status(uint32_t thread_id);
