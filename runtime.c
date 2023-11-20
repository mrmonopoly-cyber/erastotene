#include <linux/sched.h>
#include <sys/syscall.h>
#include <stddef.h>
#include <stdint.h>
#include <signal.h>

#include "dynamic_buffer.h"
#include "runtime.h"

//private
#define INITIAL_THREAD_CAPACITY 10
#define TIMEOUT 10

enum THREAD_STATUS {
    READY=0,
    WORKING,
    STOPPPING,
    ZOMBIE
};

struct thread{
    enum THREAD_STATUS status;
    struct list *channels;
    uint16_t id_thread;
    void *(*thread_fun)(void *);
};

struct stack{
    struct list *threads;
};

//ADT 
static struct stack stack ={
    .threads=NULL,
};

//functions
//for signal handling
static void attempt_termination_program()
{

}

unsigned int thread_equality(void *ele_l, void *key)
{
    struct thread ele_l_c = *(struct thread *)ele_l;
    uint16_t id_thread = *(uint16_t *)key;
    return ele_l_c.id_thread==id_thread;
}

void thread_free(void *thread)
{
    
}

struct thread *new_thread(void *(*thread_fun)(void *))
{
    struct thread new_thread;
    uint32_t num_thread_in_stack = list_get_num_ele(stack.threads);
    if(!stack.threads){
        stack.threads=new_list(INITIAL_THREAD_CAPACITY, sizeof(struct thread), 
                thread_equality, thread_free, NULL);
    }

    new_thread.id_thread=num_thread_in_stack;
    new_thread.thread_fun=thread_fun;
    new_thread.status=READY;
    new_thread.channels=NULL;
    return list_add_element(stack.threads, &new_thread);
}

void thread_start(struct thread *this)
{
    
}

void thread_wait(struct thread *this)
{

}
void thread_kill(struct thread *this)
{

}

int thread_add_channel(struct thread * this)
{
    return 0;
}


