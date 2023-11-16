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
    
    int fd_server,fd_client;
    socklen_t client_len;
    struct sockaddr_un addr_server, addr_client;
    char socket_path[55] = {};
    sprintf(socket_path, "%d", 3);
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
    

    if(listen(fd_server, 1) <0){
        return error();
    }
    printf("waiting connections\n");
    client_len = sizeof(addr_client);
    if( (fd_client=accept(fd_server, (struct sockaddr *)&addr_client,&client_len )) < 0){
        return error();
    }
    printf("connection accepted\n");
    
 

    return 0;
}
