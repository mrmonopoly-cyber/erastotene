#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "dynamic_buffer.h"
#include "runtime.h"

#define INITIAL_THREAD_NUMBER 10
#define MAX_SOCKET_NAME_LENGTH 4

enum THREAD_STATUS {
    WORKING,
    READY,
    KILLED,
    TO_KILL
};

struct thread{
    enum THREAD_STATUS status;
    pthread_t thread_id;
    void *(*thread_fun)(void *);
    void *args;
};

struct connection{
    struct thread *tread1;
    struct thread *tread2;
    int fd_connection;
};

struct stack{
    struct list *threads;
    struct list *connections;
    unsigned int number_of_connections;
};

//private
static struct stack stack ={
    .threads=NULL,
    .connections=NULL,
    .number_of_connections=0,
};

unsigned int comparison_function(void *el1,void *el2){
    struct thread t1 = *(struct thread *)el1;
    struct thread t2 = *(struct thread *)el2;
    return t1.thread_id < t2.thread_id;
}

static int start_thread(struct thread * thread){
    if(pthread_create(&thread->thread_id, NULL , thread->thread_fun, thread->args)!=0){
        fprintf(stderr, "runtime error, thread creation failed\n");
        return -1;
    }
    thread->status=WORKING;
    return 0;
}
//public

struct thread *new_thread(void *(*thread_fun)(void *),void *args)
{
    if(!stack.threads){
        stack.threads=new_list(INITIAL_THREAD_NUMBER, sizeof(struct stack), comparison_function);
    }

    struct thread * new_thread=malloc(sizeof(*new_thread));

    new_thread->status=READY;
    new_thread->thread_fun = thread_fun;
    new_thread->args=args;
    list_add_element(stack.threads,new_thread);

    return new_thread;
}

void start_thread_sync(struct thread * thread)
{
    if(!start_thread(thread)){
        pthread_join(thread->thread_id, NULL);
    }
}

void start_thread_async(struct thread * thread)
{
    if(!start_thread(thread)){
        pthread_detach(thread->thread_id);
    }
}

int new_connection(struct thread *thread1, struct thread *thread2)
{
    int fd_connection=-1;
    char socket_name[MAX_SOCKET_NAME_LENGTH] = {};

    sprintf(socket_name,"%d", stack.number_of_connections);
    fd_connection=open(socket_name,O_CREAT | O_WRONLY);

    if(fd_connection<0){
        return fd_connection;
    }

    stack.number_of_connections++;
    struct connection *new_connection=malloc(sizeof(*new_connection));
    new_connection->tread1=thread1;
    new_connection->tread2=thread2;
    new_connection->fd_connection=fd_connection;
    list_add_element(stack.connections, new_connection);

    return fd_connection;
}

void recv(int connection,void *buffer, unsigned int buffer_size)
{

}

void send(int connection,void *buffer, unsigned int buffer_size)
{

}

void IO_management(enum IO mode, void *buffer, unsigned int buffer_size)
{

}
