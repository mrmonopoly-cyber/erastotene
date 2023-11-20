#pragma once

#include <stdint.h>

typedef enum THREAD_STATUS {
    READY=0,
    WORKING,
    STOPPPING,
    ZOMBIE
}THREAD_STATUS;

struct thread;

struct thread *new_thread(int (*thread_fun)(void *));

void thread_start(struct thread *this,void *args);

void thread_wait(struct thread *this);

int thread_suspend(struct thread* this);

int thread_resume(struct thread* this);

int thread_kill(struct thread* this);

THREAD_STATUS thread_status(struct thread* this);
