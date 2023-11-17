#pragma once

enum IO{
    INPUT,
    OUTPUT
};

struct thread;

struct thread *new_thread(void *(*thread_fun)(void *));
void start_thread_sync(struct thread * this,void *args);
void start_thread_async(struct thread * this,void *args);
int add_channel(struct thread * this);
void read_channel(struct thread *this,unsigned char channel,void *buffer,unsigned int  buffer_size);
void send_to_channel(struct thread *this,unsigned char channel, void *buffer, unsigned int buffer_size);
