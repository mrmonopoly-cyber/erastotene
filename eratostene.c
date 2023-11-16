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
    unsigned int parent_fd;
};

inline int error(){

    fprintf(stderr, "error %d\n",__LINE__);
    return -1;
}

int setup_server(int prime_number){
    int fd_server;
    struct sockaddr_un addr_server;
    char socket_path[108] = {};
    sprintf(socket_path, "%d", prime_number);
    fd_server = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(fd_server <0){
        return error();
    }
    
    memset(&addr_server, 0, sizeof(addr_server));
    addr_server.sun_family = AF_LOCAL;
    memcpy(addr_server.sun_path, socket_path, sizeof(socket_path)-1);

    unlink(socket_path);
    if(bind(fd_server, (struct sockaddr *) &addr_server, sizeof(addr_server)) < 0){
        return error();
    }
    


    return fd_server;
}

int wait_connection(int fd_server){
    struct sockaddr_un addr_client;
    socklen_t client_len;
    int fd_client;

    if(listen(fd_server, 1) <0){
        return error();
    }
    printf("waiting connections\n");
    client_len = sizeof(addr_client);
    if( (fd_client=accept(fd_server, (struct sockaddr *)&addr_client,&client_len )) < 0){
        return error();
    }
    printf("connection accepted\n");

    return fd_client;

}

int connect_to_parent(int prime_number){
    int fd_client;
    socklen_t client_len;
    struct sockaddr_un addr_client;
    char socket_path[108] = {};
    sprintf(socket_path, "%d", prime_number);

    fd_client = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(fd_client <0){
        return error();
    }
    
    memset(&addr_client, 0, sizeof(addr_client));
    addr_client.sun_family = AF_LOCAL;
    memcpy(addr_client.sun_path, socket_path, sizeof(addr_client.sun_path)-1);

    client_len = sizeof(addr_client);
    if(connect(fd_client, (struct sockaddr *)&addr_client, client_len) <0){
        printf("error connecting to parent\n");
        return error();
    }
    return fd_client;
    
}

int possible_prime(int prime, int my_prime){
    
    return (prime%2)!=0 && abs(prime)>2 && (prime%my_prime)!=0;

}

void *eratostene(void *args){
    struct eratostene_input *input = (struct eratostene_input *) args;
    struct eratostene_input input_to_child;
    struct thread *child=NULL;
    int parent_fd=STDIN; 
    int fd_my;
    int fd_child;
    int data_read;
    fd_my=setup_server(input->prime_number);       
    if(input->prime_number != 3){
        parent_fd=connect_to_parent(input->prime_number);
    }
    
    while (1) {
        data_read = read_data(parent_fd);
        if(possible_prime(data_read, input->prime_number)){
            if(!child){ //child already present
                input_to_child.parent_fd=input->prime_number;
                input_to_child.prime_number=data_read;
                child = new_thread(eratostene,&input_to_child);
                start_thread(child);
                fd_child = wait_connection(fd_my);
            };
            send(fd_child, &data_read,sizeof(data_read), 0);
        }
    }
    

    
    return NULL;
}

int main(){

    struct eratostene_input input =
    {
        .parent_fd=STDIN,
        .prime_number=3.
    };
    eratostene(&input);
    return 0;
}
