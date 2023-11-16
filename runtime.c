#pragma once
#include "runtime.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_THREAD_NUMBER 10

enum THREAD_STATUS {
    WORKING,
    READY,
    KILLED,
    TO_KILL
};

struct thread{
    pthread_t thread_id;
    enum THREAD_STATUS status;
    void *(*thread_fun)(void *);
};

struct program_status{
    struct thread threads[INITIAL_THREAD_NUMBER];
    unsigned int num_thread;
    unsigned int next_free;
    struct program_status *next_batch;
};

//private
static struct program_status env = {
    .num_thread=INITIAL_THREAD_NUMBER,
    .next_free=0,
};

void *default_routine(void * args)
{
    return NULL;
}

//public
struct thread *new_thread(void *(*thread_fun)(void *))
{
    unsigned int index=env.next_free;
    env.next_free++;
    env.threads[index].status=READY;
    env.threads[index].thread_fun = thread_fun;
    return &env.threads[index];
}

int start_thread(struct thread * thread){
    if(pthread_create(&thread->thread_id, NULL , thread->thread_fun, NULL)!=0){
        fprintf(stderr, "runtime error, thread creation failed\n");
        return -1;
    }
    thread->status=WORKING;
    return 0;
}

int kill_thread(struct thread *thread)
{
    return 0;
}
