#include "dynamic_buffer.h"
#include "runtime.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

#define FIRST_PRIME 3

struct eratostene_input{
    struct thread *execute_thread;
    int fd_channel;
    int node_prime;
};

unsigned int comp_int(void *int1, void *int2)
{
    int a = *(int *)int1;
    int b = *(int *)int2;
    return a==b;
}

struct list *result=NULL;

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
                list_add_element(result, &buffer);
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
    int buffer=FIRST_PRIME;
    result = new_list(1, sizeof(buffer), comp_int);
    list_add_element(result, &buffer);
    printf("waiting number\n");
    scanf("%d",&buffer);
    fflush(stdin);
    for(int i=4;i<buffer;i++){
        send_to_channel(start,input_channel, &i,sizeof(i));
        sleep(1);
    }
    printf("printing current numbers\n");
    print_list(result);
    printf("\n");
    return 0;
}
