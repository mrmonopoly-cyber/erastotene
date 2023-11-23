#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define STACK_SIZE (1024 * 1024)  // 1 MB stack size

int thread_function(void *arg) {
    printf("Hello from the new thread!\n");
    return 0;
}

int main() {
    // Allocate stack for the new thread using mmap
    char *stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (stack == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Create a new thread using clone
    pid_t pid = clone(thread_function, stack + STACK_SIZE, CLONE_VM | CLONE_THREAD | CLONE_SIGHAND, NULL);
    if (pid == -1) {
        perror("clone");
        exit(EXIT_FAILURE);
    }

    // Wait for the thread to finish
    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }

    if (WIFEXITED(status)) {
        printf("Child exited with status %d\n", WEXITSTATUS(status));
    } else {
        printf("Child process did not exit normally\n");
    }

    printf("Main thread exiting.\n");

    // Unmap the allocated stack
    if (munmap(stack, STACK_SIZE) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    return 0;
}

