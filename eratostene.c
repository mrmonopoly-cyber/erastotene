#include "runtime.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#define FIRST_PRIME 3

struct eratostene_input{
    struct thread *execute_thread;
    int fd_channel;
    int node_prime;
};

int possible_prime(int prime, int my_prime){
    return (prime%2)!=0 && abs(prime)>2 && (prime%my_prime)!=0;
}

void *eratostene(void *args){
    struct eratostene_input *in = (struct eratostene_input *) args;
    struct eratostene_input in_child;
    struct thread *this = in->execute_thread;
    struct thread *child = NULL;
    int channel_input = in->fd_channel;
    int channel_child;
    int node_prime =in->node_prime;
    unsigned int buffer;

    while (1) {
        read_channel(this,channel_input, &buffer, sizeof(buffer));
        printf("number of node %d\n",node_prime);
        if(possible_prime(buffer, node_prime)){
            in_child.node_prime=buffer;
            in_child.fd_channel=channel_child;
            in_child.execute_thread=child;
            if(!child){
                child = new_thread(eratostene);
                channel_child = add_channel(child);
                in_child.fd_channel=channel_child;
                in_child.execute_thread=child;
                start_thread_async(child, &in_child);
            }else {
                send_to_channel(child,channel_child, &buffer, sizeof(buffer));
            }
        }
    }
    return NULL;
}

int main(){
    struct thread *start = new_thread(eratostene);
    struct eratostene_input era;
    int input_channel = add_channel(start);
    era.fd_channel = input_channel;
    era.execute_thread = start;
    era.node_prime=FIRST_PRIME;
    start_thread_async(start,&era);
    int buffer;
    while (1) {
        printf("waiting number\n");
        scanf("%d",&buffer);
        fflush(stdin);
        send_to_channel(start,input_channel, &buffer,sizeof(buffer));
        printf("printing current number\n");
    }
    return 0;
}
