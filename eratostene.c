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
    struct eratostene_input *input = (struct eratostene_input*) args;
    struct eratostene_input input_data_child;
    struct thread *current_thread=NULL;
    struct thread *child=NULL;
    int node_prime;
    int connection_parent,connection_child;
    unsigned int buffer;

    node_prime = input->prime_number;
    current_thread = input->current_thread;
    connection_parent=input->connection_parent;

    while (1) {
        switch (node_prime) {
            case FIRST_PRIME:
                IO_management(INPUT, &buffer, sizeof(buffer));
                break;
            default:
                recv(connection_parent, &buffer, sizeof(buffer));
                break;
        }
        if(possible_prime(buffer, node_prime)){
            if(!child){ //child not present
                input_data_child.prime_number=buffer;
                input_data_child.current_thread=current_thread;
                child=new_thread(eratostene, &input_data_child);
                start_thread_async(child);
                connection_child = new_connection(current_thread, child);
            }
            send(connection_child, &buffer, sizeof(buffer));
        }
    }
    

    
    return NULL;
}

int main(){
    struct eratostene_input input ={
        .prime_number=FIRST_PRIME,
    };
    struct thread *start = new_thread(eratostene, &input);
    input.current_thread=start;

    start_thread_sync(start);
    return 0;
}
