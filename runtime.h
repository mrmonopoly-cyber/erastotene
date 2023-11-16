struct thread;

struct thread *new_thread(void *(*thread_fun)(void *));
int start_thread(struct thread * thread);
int kill_thread(struct thread *thread);
