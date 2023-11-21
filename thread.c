#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdint.h>
#include <stddef.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include "thread.h"
#include "dynamic_buffer.h"

#define STACK_SIZE 1024 * 1024

struct thread{
    enum THREAD_STATUS status;
    uint16_t id_thread;
    void *thread_stack_top;
    int (*thread_fun)(void *);
};

static unsigned int comparison_function_thread(void *thread_list,void *key)
{
    struct thread *this = (struct thread *) thread_list;
    uint16_t id_thread = *(uint16_t *) key;

    return id_thread==this->id_thread;
}

static int signal_thread(struct thread *this, uint8_t signal)
{
    int error=0;
    if(this->id_thread<=0){
        return -1;
    }
    if((error=kill(this->id_thread, signal))){
        fprintf(stderr, "failed to %d thread with id: %d\n",signal,this->id_thread);
    }
    this->status=ZOMBIE;
    return error;
}

//public
struct thread *new_thread(int (*thread_fun)(void *))
{
    void * stack = mmap(NULL,STACK_SIZE, PROT_READ | PROT_WRITE , MAP_ANON | MAP_PRIVATE, -1 ,0);   

    struct thread * new_thread = mmap(NULL,sizeof(*new_thread), 
            PROT_READ | PROT_WRITE , MAP_ANON | MAP_PRIVATE, -1 ,0);   

    if (stack == MAP_FAILED || new_thread == MAP_FAILED) {
        perror("mmap failed");
        // Gestisci l'errore e restituisci un valore o esci
    }

    new_thread->thread_fun=thread_fun;
    new_thread->thread_stack_top=stack+STACK_SIZE;
    new_thread->status=READY;

    return new_thread;
}

void thread_start(struct thread * this,void *args)
{
    this->id_thread=clone(this->thread_fun, this->thread_stack_top,  
            CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD,args);
     this->status=WORKING;
}

void thread_wait(struct thread * this)
{
    int32_t status;
    if(this->id_thread){
        waitpid(this->id_thread, &status, 0); //0 indicate no options
    }
}

inline int thread_suspend(struct thread * this)
{
    return signal_thread(this, SIGSTOP );
}

inline int thread_resume(struct thread * this)
{
    return signal_thread(this, SIGCONT);
}

inline int thread_kill(struct thread * this)
{
    int error;
    this->status=STOPPPING;
    if((error=signal_thread(this, SIGKILL))){
        return error;
    }
    munmap(this->thread_stack_top-STACK_SIZE,STACK_SIZE);
    munmap(this,sizeof(*this));
    
    return error;
}

THREAD_STATUS thread_status(struct thread * this)
{
    return this->status;
}
