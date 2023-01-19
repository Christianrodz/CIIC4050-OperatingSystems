#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <sys/un.h>
#include <signal.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include <errno.h>
// Prototypes for internal functions and utilities
void error(const char *fmt, ...);
int runClient(char *clientName, int numMessages, char **messages);
int runServer();
void serverReady(int signal);

bool serverIsReady = false;

// Prototypes for functions to be implemented by students
void server();
void client(char *clientName, int numMessages, char *messages[]);

void verror(const char *fmt, va_list argp)
{
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, argp);
    fprintf(stderr, "\n");
}

void error(const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    verror(fmt, argp);
    va_end(argp);
    exit(1);
}

int runServer(int port) {
    int forkPID = fork();
    if (forkPID < 0)
        error("ERROR forking server");
    else if (forkPID == 0) {
        server();
        exit(0);
    } else {
        printf("Main: Server(%d) launched...\n", forkPID);
    }
    return forkPID;
}

int runClient(char *clientName, int numMessages, char **messages) {
    fflush(stdout);
    printf("Launching client %s...\n", clientName);
    int forkPID = fork();
    if (forkPID < 0)

        error("ERROR forking client %s", clientName);
    else if (forkPID == 0) {
        client(clientName, numMessages, messages);
        exit(0);
    }
    return forkPID;
}

void serverReady(int signal) {
    serverIsReady = true;
}

#define NUM_CLIENTS 2
#define MAX_MESSAGES 5
#define MAX_CLIENT_NAME_LENGTH 10

struct client {
    char name[MAX_CLIENT_NAME_LENGTH];
    int num_messages;
    char *messages[MAX_MESSAGES];
    int PID;
};

// Modify these to implement different scenarios
struct client clients[] = {
        {"Uno", 3, {"Mensaje 1-1", "Mensaje 1-2", "Mensaje 1-3"}},
        {"Dos", 3, {"Mensaje 2-1", "Mensaje 2-2", "Mensaje 2-3"}}
};

int main() {
    signal(SIGUSR1, serverReady);
    int serverPID = runServer(getpid());
    while(!serverIsReady) {
        sleep(1);
    }
    printf("Main: Server(%d) signaled ready to receive messages\n", serverPID);

    for (int client = 0 ; client < NUM_CLIENTS ; client++) {
        clients[client].PID = runClient(clients[client].name, clients[client].num_messages,
                                        clients[client].messages);
    }

    for (int client = 0 ; client < NUM_CLIENTS ; client++) {
        int clientStatus;
        printf("Main: Waiting for client %s(%d) to complete...\n", clients[client].name, clients[client].PID);
        waitpid(clients[client].PID, &clientStatus, 0);
        printf("Main: Client %s(%d) terminated with status: %d\n",
               clients[client].name, clients[client].PID, clientStatus);
    }

    printf("Main: Killing server (%d)\n", serverPID);
    kill(serverPID, SIGINT);
    int statusServer;
    pid_t wait_result = waitpid(serverPID, &statusServer, 0);
    printf("Main: Server(%d) terminated with status: %d\n", serverPID, statusServer);
    return 0;
}



//*********************************************************************************
//**************************** EDIT FROM HERE *************************************
//#you can create the global variables and functions that you consider necessary***
//*********************************************************************************

#define PORT_NUMBER 44142
#define SA struct sockaddr

bool serverShutdown = false;
int serverPIDs[NUM_CLIENTS];

void shutdownServer(int signal) {
    //Indicate that the server has to shutdown
    //Wait for the children to finish
    //Exit
}

void client(char *clientName, int numMessages, char *messages[])
{
    char buffer[256];
    int sockfd, n;
    struct sockaddr_in  servaddr;
    struct hostent *server;
    //Open the socket
    if( (sockfd = socket(AF_INET, SOCK_STREAM,0)) < 0) {
        error("Error on socket creation!");
    }
    server = gethostbyname("localhost");
    if(server == NULL){
        fprintf(stderr,"No such host error");
        exit(0);
    }
    
    //Connect to the server
    bzero((char *) &servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&servaddr.sin_addr.s_addr, server->h_length);
    servaddr.sin_port = htons(PORT_NUMBER);
    if( connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0){
        error("Connection failed");
    } 

    //For each message, write to the server and read the response
    for(int i = 0; i < numMessages; i++){
       n = write(sockfd, messages[i], strlen(messages[i]));
       if(n < 0) error("Socket Write Error");
       bzero(buffer,256);
       n = read(sockfd, buffer,255);
       if(n < 0) error("Socket Read Error");
       //Accept connection and create a child proccess to read and write 
       printf("Client %s(%d): Return message: %s\n", clientName, getpid(), buffer);
       fflush(stdout);
       sleep(1);
    }

    //Close socket
    close(sockfd);
    exit(0);
}

void server()
{
    char buffer[256];
    int listenfd, connfd, n;
    socklen_t clilen;
    struct sockaddr_in servaddr, cliaddr;
    
    //Handle SIGINT so the server stops when the main process kills it
    signal(SIGINT, shutdownServer);
    //Open the socket
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        error("socket open error");
    }

    //Init servaddr to bind
    bzero((char *) &servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT_NUMBER);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    //Bind the socket

    if((bind(listenfd, (SA *) &servaddr, sizeof(servaddr) )) < 0){
        error("Error when binding socket");
    }
    listen(listenfd,5);
    //Signal server is ready
    kill(getppid(), SIGUSR1);

    //Accept connection and create a child proccess to read and write 
    for(int i = 0; i < NUM_CLIENTS; i++){
        clilen = sizeof(cliaddr);
        connfd = accept( listenfd, (SA *) &cliaddr, &clilen);
        if(connfd < 0) error("Error accepting");

        serverPIDs[i] = fork();
        if (serverPIDs[i] < 0) error("error forking server: %d", serverPIDs[i]);
        else if(serverPIDs[i] == 0){//Child Process
            for(int j = 0; j < MAX_MESSAGES; j++){
                //Read Msg
                bzero(buffer,sizeof(buffer));
                n = read(connfd, buffer,255);
                if(n < 0) {
                    error("Error reading msg");
                }else if(n==0){break;}
            
                printf("Server child(%d): got message: %s\n", getpid(), buffer); //expected output

                n = write(connfd, buffer, n);
                if (n < 0) error("ERROR writing to socket");
            }
            close(connfd);
            exit(0);
            
        }else{ }//parent does nothing

    }
}
