#pragma once

enum IO{
    INPUT,
    OUTPUT
};

struct thread;

struct thread *new_thread(void *(*thread_fun)(void *));
void thread_start(struct thread *this);
void thread_wait(struct thread *this);
void thread_kill(struct thread *this);
int thread_add_channel(struct thread * this);
void tread_read_channel(struct thread *this,unsigned char channel,void *buffer,unsigned int  buffer_size);
void thread_send_to_channel(struct thread *this,unsigned char channel, void *buffer, unsigned int buffer_size);
