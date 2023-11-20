#include <stdio.h>
#define _GNU_SOURCE
#include <sched.h>
#include <stdint.h>
#include <stddef.h>
#include <signal.h>

#include "thread.h"
#include "dynamic_buffer.h"

#define INITIAL_CAPACITY_SUSPEND_QUEUE 10
#define INITIAL_CAPACITY_WORKING_QUEUE 4

struct thread{
    enum THREAD_STATUS status;
    uint16_t id_thread;
    uint16_t thread_timeout;
    void *(*thread_fun)(void *);
};

static uint64_t thrad_id=0;
static struct list *suspended_thread;
static struct list *working_thread;


static unsigned int comparison_function_thread(void *thread_list,void *key)
{
    struct thread *this = (struct thread *) thread_list;
    uint16_t id_thread = *(uint16_t *) key;

    return id_thread==this->id_thread;
}

static void thread_free(void *thread){


}
static void check_initialization_suspend_queue()
{
    if(!suspended_thread){
        suspended_thread=new_list(INITIAL_CAPACITY_SUSPEND_QUEUE,sizeof(struct thread), 
                comparison_function_thread, thread_free, NULL);
    }

}

static void check_initialization_working_queue()
{
    if(!suspended_thread){
        working_thread=new_list(INITIAL_CAPACITY_WORKING_QUEUE,sizeof(struct thread), 
                comparison_function_thread, thread_free, NULL);
    }

}
//public
uint32_t new_thread(void *(*thread_fun)(void *), uint16_t timeout)
{

    return 0;
}

void thread_start(uint32_t thread_id);

void thread_wait(uint32_t thread_id);

void thread_suspend(uint32_t thread_id)
{
    struct thread *this;
    uint8_t index_ele=-1;

    if(!working_thread){
        return;
    }

    this=list_search_element(working_thread, &thread_id);
    if(!this){
        fprintf(stderr, "failed to suspend thread, no working thread present\n");
        return;
    }

    check_initialization_suspend_queue();
    
    if ((index_ele=list_get_index_element(working_thread, &thread_id))<0){
        fprintf(stderr, "failed to suspend thread, thread not found\n");
        return;
    }
    
    this=list_get_at_index(working_thread, index_ele);
    
    list_add_element(suspended_thread, this);
    if(kill(thread_id, SIGSTOP)){
        fprintf(stderr, "failed to suspend thread with id: %d\n",thread_id);
        return;
    }

    if(list_clear_element(working_thread, &thread_id)){
        fprintf(stderr, "failed to suspend thread, thread not found\n");
        return;
    }
}

void thread_resume(uint32_t thread_id);

void thread_kill(uint32_t thread_id);

THREAD_STATUS thread_status(uint32_t thread_id);
