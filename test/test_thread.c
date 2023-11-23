#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>

#include "include/thread.h"


int t_fun_1(void * args){
    unsigned int *input = (unsigned int *)args;
    printf("thread 1 alive\n");
    while (1) {
     // if(!((*input)%2)){ 
     if(1){
        printf("thread 1 turn\n");
        // sleep(2);
        (*input)++;
     }
    }
    return 0;
}

int t_fun_2(void * args){
    unsigned int *input = (unsigned int *)args;
    printf("thread 2 alive\n");
    while (1) {
     if((*input)%2){
        printf("thread 2 turn\n");
        sleep(2);
        (*input)++;
     }
    }
    return 0;
}


int main(){

    unsigned int input =0;

    struct thread *thread_1;
    struct thread *thread_2;

    thread_1 = new_thread(t_fun_1);
    // thread_2 = new_thread(t_fun_2);
    
    thread_start(thread_1,&input);
    // thread_start(thread_2,&input);

    sleep(10);

    printf("suspending thread 1\n");
    thread_suspend(thread_1);
    sleep(8);
    printf("resuming thread 1\n");

    input=1;
    thread_wait(thread_1);
    // thread_wait(thread_2);
    return 0;
}

//
// static int child_func(void* arg) {
//   char* buf = (char*)arg;
//   printf("Child sees buf = \"%s\"\n", buf);
//   strcpy(buf, "hello from child");
//   sleep(5);
//   return 0;
// }
//
// int main(int argc, char** argv) {
//   // Allocate stack for child task.
//   const int STACK_SIZE = 65536;
//   char* stack = malloc(STACK_SIZE);
//   if (!stack) {
//     perror("malloc");
//     exit(1);
//   }
//
//   // When called with the command-line argument "vm", set the CLONE_VM flag on.
//   unsigned long flags = 0;
//   if (argc > 1 && !strcmp(argv[1], "vm")) {
//     flags |= CLONE_VM;
//   }
//
//   char buf[100];
//   strcpy(buf, "hello from parent");
//   if (clone(child_func, stack + STACK_SIZE, flags | SIGCHLD, buf) == -1) {
//     perror("clone");
//     exit(1);
//   }
//
//   int status;
//   if (wait(&status) == -1) {
//     perror("wait");
//     exit(1);
//   }
//
//   printf("Child exited with status %d. buf = \"%s\"\n", status, buf);
//   return 0;
// }
