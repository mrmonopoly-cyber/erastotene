#pragma once
struct thread;

struct thread *new_thread(void *(*thread_fun)(void *),void *args);
int start_thread(struct thread * thread);
int kill_thread_sync(struct thread *thread);
int kill_thread_async(struct thread *thread);
int read_data(unsigned int fd);
