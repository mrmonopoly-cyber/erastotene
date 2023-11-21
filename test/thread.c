#include "../thread.h"
#include <stdio.h>
#include <unistd.h>

int shared_value =0;

int print_args(void *a){
    while (1) {
        printf("thread %d with shared value %d\n",*(int *)a, shared_value);
        shared_value++;
        sleep(1);
    }
}

int main(){
    int a=1;
    int b=2;
    struct thread *thread_1 = new_thread(print_args);
    struct thread *thread_2 = new_thread(print_args);
    
    thread_start(thread_1,&a);
    thread_start(thread_2,&b);

    sleep(5);

    thread_suspend(thread_1);
    printf("thread 1 suspended\n");
    thread_wait(thread_2);

    thread_resume(thread_1);
    printf("thread 1 resumed\n");

    sleep(5);
    thread_wait(thread_1);
    return 0;
}
