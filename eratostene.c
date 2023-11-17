#include "runtime.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#define FIRST_PRIME 3

struct eratostene_input{
    unsigned long long prime_number;
    struct thread *current_thread;
    struct thread *parent_thread;
    int connection_parent;
};

int possible_prime(int prime, int my_prime){
    return (prime%2)!=0 && abs(prime)>2 && (prime%my_prime)!=0;
}

void *eratostene(void *args){
       

    
    return NULL;
}

int main(){
    struct thread *start = new_thread(eratostene, NULL);
    start_thread_sync(start);
    return 0;
}
