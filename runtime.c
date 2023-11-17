#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "dynamic_buffer.h"
#include "runtime.h"

#define INITIAL_THREAD_NUMBER 10
#define MAX_SOCKET_NAME_LENGTH 8

enum THREAD_STATUS {
    WORKING,
    READY,
    KILLED,
    TO_KILL
};
struct channel{
    int name_buffer;
};

struct thread{
    enum THREAD_STATUS status;
    struct list *channels;
    unsigned char num_channels;
    pthread_t thread_id;
    unsigned int id_stack;
    void *(*thread_fun)(void *);
};

struct stack{
    struct list *threads;
    unsigned char num_thread;
};

//private
static struct stack stack ={
    .threads=NULL,
    .num_thread=0,
};

unsigned int comparison_function_thread(void *el1,void *el2){
    struct thread t1 = *(struct thread *)el1;
    struct thread t2 = *(struct thread *)el2;
    return t1.thread_id == t2.thread_id;
}

unsigned int comparison_function_channel(void *a,void *channel_name){
    struct channel * a_c =(struct channel *) a;
    int cha_c = *(int *)channel_name;

    return a_c->name_buffer==cha_c;
}

static int start_thread(struct thread * thread,void *args){
    if(pthread_create(&thread->thread_id, NULL , thread->thread_fun, args)!=0){
        fprintf(stderr, "runtime error, thread creation failed\n");
        return -1;
    }
    thread->status=WORKING;
    return 0;
}

static void full_name_socket(struct thread *this, char *conv_name, unsigned char offset)
{
    conv_name[0]=this->id_stack+1;
    conv_name[1]=offset+1;
}

//public
struct thread *new_thread(void *(*thread_fun)(void *))
{
    if(!stack.threads){
        stack.threads=new_list(INITIAL_THREAD_NUMBER, sizeof(struct stack), comparison_function_thread);
    }

    struct thread * new_thread=malloc(sizeof(*new_thread));

    new_thread->status=READY;
    new_thread->channels=NULL;
    new_thread->thread_fun = thread_fun;
    new_thread->id_stack=stack.num_thread;
    list_add_element(stack.threads,new_thread);
    
    stack.num_thread++;
    return new_thread;
}

void start_thread_sync(struct thread * this, void *args)
{
    if(!start_thread(this,args)){
        pthread_join(this->thread_id, NULL);
    }
}

void start_thread_async(struct thread * this, void *args)
{
    if(!start_thread(this,args)){
        pthread_detach(this->thread_id);
    }
}

int add_channel(struct thread * this)
{
    char conv_name[MAX_SOCKET_NAME_LENGTH] = {};
    full_name_socket(this,conv_name,this->num_channels);
    int real_file = open(conv_name, O_CREAT | O_RDWR,S_IRWXU);
    ftruncate(real_file, 0);
    if(real_file<0){
        fprintf(stderr, "error creating file");
        return -1;
    }
    struct channel *channel = malloc(sizeof(*channel));
    channel->name_buffer=strtol(conv_name,NULL ,10);
    if(!this->channels){
        this->channels=new_list(1, sizeof(*channel), comparison_function_channel);
    }
    list_add_element(this->channels, channel);
    this->num_channels++;
    close(real_file);
    return this->num_channels-1;
}

void read_channel(struct thread *this,unsigned char channel_name,void *buffer,unsigned int  buffer_size)
{
    char conv_name[MAX_SOCKET_NAME_LENGTH] = {};
    full_name_socket(this,conv_name,channel_name);
    int real_file = open(conv_name, O_RDONLY | O_RDWR,S_IRWXU);
    if(real_file<0){
        fprintf(stderr, "read,error opening file\n");
        return;
    }

    while(read(real_file, buffer, buffer_size)<=0){}
    ftruncate(real_file, 0);
    close(real_file);
}

void send_to_channel(struct thread *this,unsigned char channel_name, void *buffer, unsigned int buffer_size)
{
    char conv_name[MAX_SOCKET_NAME_LENGTH] = {};
    full_name_socket(this,conv_name,channel_name);
    int real_file = open(conv_name, O_RDONLY | O_RDWR,S_IRWXU);
    if(real_file<0){
        fprintf(stderr, "write,error opening file\n");
        return;
    }

    if(write(real_file, buffer, buffer_size)<0){
        fprintf(stderr, "error writing\n");
        return;
    }
  
    close(real_file);
}
