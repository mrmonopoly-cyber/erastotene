#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "dynamic_buffer.h"
#include "runtime.h"

#define INITIAL_THREAD_NUMBER 10

enum THREAD_STATUS {
    WORKING,
    READY,
    KILLED,
    TO_KILL
};

struct thread{
    enum THREAD_STATUS status;
    pthread_t thread_id;
    unsigned int num_channels;
    struct list *fd_buffer;
    void *(*thread_fun)(void *);
    void *args;
};

//private
static struct list *env = NULL;

unsigned int comparison_function(void *el1,void *el2){
    struct thread t1 = *(struct thread *)el1;
    struct thread t2 = *(struct thread *)el2;
    return t1.thread_id < t2.thread_id;
}

//public
struct thread *new_thread(void *(*thread_fun)(void *),void *args)
{
    if(!env){
        env=new_list(INITIAL_THREAD_NUMBER, sizeof(struct thread), comparison_function);
    }

    struct thread * new_thread=malloc(sizeof(*new_thread));

    new_thread->status=READY;
    new_thread->thread_fun = thread_fun;
    new_thread->args=args;
    new_thread->fd_buffer=NULL;
    new_thread->num_channels=0;
    list_add_element(env,new_thread);

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

int create_channel_thread(struct thread *thread)
{
    pthread_t channel_name = thread->thread_id + thread->num_channels;
    char name_file[2] = {};
    sprintf(name_file, "%ld", channel_name);
    int fd_buffer=open(name_file, O_CREAT | O_WRONLY);
    list_add_element(thread->fd_buffer,&fd_buffer);
    return channel_name;
}
