#pragma once

#include <stdint.h>

typedef enum THREAD_STATUS {
    READY=0,
    WORKING,
    STOPPPING,
    ZOMBIE
}THREAD_STATUS;

uint32_t new_thread(void *(*thread_fun)(void *), uint16_t timeout);

void thread_start(uint32_t thread_id);

void thread_wait(uint32_t thread_id);

void thread_suspend(uint32_t thread_id);

void thread_resume(uint32_t thread_id);

void thread_kill(uint32_t thread_id);

THREAD_STATUS thread_status(uint32_t thread_id);
