#include "runtime.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    void *args;
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
    .next_batch=NULL,
};

//public
struct thread *new_thread(void *(*thread_fun)(void *),void *args)
{
    unsigned int index=env.next_free;
    struct thread * new_thread= &env.threads[index];
    env.next_free++;
    new_thread->status=READY;
    new_thread->thread_fun = thread_fun;
    new_thread->args=args;
    return new_thread;
}

int start_thread(struct thread * thread){
    if(pthread_create(&thread->thread_id, NULL , thread->thread_fun, thread->args)!=0){
        fprintf(stderr, "runtime error, thread creation failed\n");
        return -1;
    }
    thread->status=WORKING;
    return 0;
}

int kill_thread_sync(struct thread *thread)
{
    return 0;
}

int kill_thread_async(struct thread *thread)
{
    return 0;
}

int read_data(unsigned int fd)
{
    int data;
    read(fd, &data, sizeof(data));
    return data;
}
