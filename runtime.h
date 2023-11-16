#pragma once
struct thread;

struct thread *new_thread(void *(*thread_fun)(void *),void *args);
int start_thread(struct thread * thread);
int kill_thread_sync(struct thread *thread);
int kill_thread_async(struct thread *thread);
int create_channel_thread(struct thread *thread);
void read_from_channel(struct thread *thread);
void send_to_thread(struct thread *thread, int channel_id);
