#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

inline int error(){

    fprintf(stderr, "error %d\n",__LINE__);
    return -1;
}

int main(){
    
    int fd_client;
    socklen_t client_len;
    struct sockaddr_un addr_client;
    char socket_path[55] = {};
    sprintf(socket_path, "%d", 3);

    fd_client = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(fd_client <0){
        return error();
    }
    
    memset(&addr_client, 0, sizeof(addr_client));
    addr_client.sun_family = AF_LOCAL;
    memcpy(addr_client.sun_path, socket_path, sizeof(socket_path)-1);

    client_len = sizeof(addr_client);
    if(connect(fd_client, (struct sockaddr *)&addr_client, client_len) <0){
        return error();
    }
    
    printf("connected\n");
    

    return 0;
}
