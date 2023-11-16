#include "runtime.h"

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

#define STDIN 0

struct eratostene_input{
    unsigned long long prime_number;
    struct thread *thread;
};

inline int error(){

    fprintf(stderr, "error %d\n",__LINE__);
    return -1;
}


int possible_prime(int prime, int my_prime){
    return (prime%2)!=0 && abs(prime)>2 && (prime%my_prime)!=0;
}

void *eratostene(void *args){
    struct eratostene_input *input = (struct eratostene_input*) args;
    int channel_current_thread = create_channel_thread(input->thread);
    
    while (1) {
    }
    

    
    return NULL;
}

int main(){
    int prime = 3;
    struct thread *start = new_thread(eratostene, &prime);
    return 0;
}
