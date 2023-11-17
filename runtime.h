#pragma once

enum IO{
    INPUT,
    OUTPUT
};

struct thread;

struct thread *new_thread(void *(*thread_fun)(void *),void *args);
void start_thread_sync(struct thread * thread);
void start_thread_async(struct thread * thread);
int new_connection(struct thread *thread1, struct thread *thread2);
void recv(int connection,void *buffer, unsigned int buffer_size);
void send(int connection,void *buffer, unsigned int buffer_size);
void IO_management(enum IO mode, void *buffer, unsigned int buffer_size);
