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
#define TIMEOUT 10

enum THREAD_STATUS {
    WORKING,
    READY,
    KILLED,
    ZOMBIE
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
    unsigned char num_zombie;
};

//private
static struct stack stack ={
    .threads=NULL,
    .num_thread=0,
    .num_zombie=0
};

static void full_name_socket(struct thread *this, char *conv_name, unsigned char offset)
{
    conv_name[0]=this->id_stack+1;
    conv_name[1]=offset+1;
}

unsigned int comparison_function_thread(void *el1,void *el2){
    struct thread t1 = *(struct thread *)el1;
    unsigned int key2 = *(int *)el2;
    return t1.thread_id == key2;
}

unsigned int comparison_function_channel(void *a,void *channel_name){
    struct channel * a_c =(struct channel *) a;
    int cha_c = *(int *)channel_name;

    return a_c->name_buffer==cha_c;
}

void free_thread(void *ele)
{
    struct thread *this = (struct thread *)ele;
    struct channel *channel = NULL;
    pthread_cancel(this->thread_id);
    char conv_name[MAX_SOCKET_NAME_LENGTH]; 
    for(int i=0;i<this->num_channels;i++){
        channel = (struct channel *)list_get_at_index(this->channels, i);
        full_name_socket(this, conv_name, channel->name_buffer);
        unlink(conv_name);
    }
    list_clear_all(this->channels);
}

void free_channel(void *ele)
{

}
static int start_thread(struct thread * thread,void *args){
    if(pthread_create(&thread->thread_id, NULL , thread->thread_fun, args)!=0){
        fprintf(stderr, "runtime error, thread creation failed\n");
        return -1;
    }
    thread->status=WORKING;
    return 0;
}


static void attempt_close_the_program()
{
    unsigned char i=0;
    
    if(stack.num_zombie!=stack.num_thread){
        return;
    }

    for(i=0;i<stack.num_thread;i++){
        list_remove_last_element(stack.threads);
    }
}

static void request_termination(struct thread *this)
{
    this->status=ZOMBIE;
    stack.num_zombie++;
    attempt_close_the_program();
    
}

static void remove_termination(struct thread *this)
{
    if(this->status==ZOMBIE){
        this->status=WORKING;
        stack.num_zombie--;
    }
}

static void read_timeout(struct thread *this, unsigned int fd, void *buffer, 
        unsigned int buffer_size)
{
    unsigned int time=0;
    while(read(fd, buffer, buffer_size)<=0 && time<TIMEOUT){
        sleep(1);
        time++;
    }

    switch (time) {
        case TIMEOUT:
            request_termination(this);
            read_timeout(this, fd, buffer, buffer_size);
            break;
        default:
            remove_termination(this);
            break;
    }
}
//public
struct thread *new_thread(void *(*thread_fun)(void *))
{
    if(!stack.threads){
        stack.threads=new_list(1, sizeof(struct stack), 
                comparison_function_thread,
                free_thread);
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

void wait_thread(struct thread *this)
{
    pthread_join(this->thread_id,NULL);
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
        this->channels=new_list(1, sizeof(*channel), 
                comparison_function_channel, free_channel);
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
    
    read_timeout(this, real_file, buffer, buffer_size);

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
