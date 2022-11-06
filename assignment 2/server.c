// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 8080

int set_socket_func();

int main(int argc, char const *argv[])
{
    //int server_fd, new_socket, valread;
    //struct sockaddr_in address;
    //int opt = 1;
    //int addrlen = sizeof(address);

    int valread;
    int new_socket = set_socket_func();
	
    char buffer[102] = {0};
    char *hello = "Hello from server";
    struct passwd* pwd;
    
    // adding additional code for assignment
    // Getting the user id for nobody
    
    // Doing the fork process
    printf("before fork %d \n", getuid());
    pid_t forked_process, pid;
    forked_process = fork();
    printf("forked_process: %d \n",forked_process);
    if (forked_process < 0)
    {
	perror("forking process failed");
	exit(EXIT_FAILURE);
    }
    //Dropping priviliges for nobody
    if (forked_process == 0)
    {
	printf("forking successful \n");
	//using setuid to drop priviliges for nobody user
	pwd = getpwnam("nobody");
	pid = setuid(pwd->pw_uid);
	printf("child_uid : %d \n", getuid());
	printf("nobody_uid : %d \n", pwd->pw_uid);
	printf("after setuid : %d \n", pid);
	if (pid == -1)
	{
	    perror("privilege drop failure");
	    exit(EXIT_FAILURE);
	}
    	valread = read( new_socket , buffer, 1024);
    	printf("%s\n",buffer);
    	send(new_socket , hello , strlen(hello) , 0 );
    	printf("Hello message sent\n");
    }	
    return 0;
}

// Second assignment start
// Adding new function to set new socket 
int set_socket_func(){
	
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    printf("execve=0x%p\n", execve);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 80
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    return new_socket;                                                                                                      
}
