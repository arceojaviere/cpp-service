#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

namespace myservice{
    static int SOCKET_QUEUE_SIZE = 10;
    static int LISTEN_ERROR = -1;
    static int ACCEPT_ERROR = -2;
    class MyTCPService{
        public: 
            void startup(short portnumber){
                
                int sockfd, newsockfd;
                socklen_t clilen;
                char buffer[256];
                struct sockaddr_in serv_addr, cli_addr;
                int n;

                // create a socket
                // socket(int domain, int type, int protocol)
                sockfd =  socket(AF_INET, SOCK_STREAM, 0);
                if (sockfd < 0) 
                    std::cout << "ERROR opening socket";

                // clear address structure
                bzero((char *) &serv_addr, sizeof(serv_addr));

                portnumber;

                /* setup the host_addr structure for use in bind call */
                // server byte order
                serv_addr.sin_family = AF_INET;  

                // automatically be filled with current host's IP address
                serv_addr.sin_addr.s_addr = INADDR_ANY;  

                // convert short integer value for port must be converted into network byte order
                serv_addr.sin_port = htons(portnumber);

                // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
                // bind() passes file descriptor, the address structure, 
                // and the length of the address structure
                // This bind() call will bind  the socket to the current IP address on port, portno
                if (bind(sockfd, (struct sockaddr *) &serv_addr,
                        sizeof(serv_addr)) < 0){
                    std::cerr << "ERROR on binding\n";
                    exit(LISTEN_ERROR);
                }

                // This listen() call tells the socket to listen to the incoming connections.
                // The listen() function places all incoming connection into a backlog queue
                // until accept() call accepts the connection.
                // Here, we set the maximum size for the backlog queue to 5.
                listen(sockfd,SOCKET_QUEUE_SIZE);

                clilen = sizeof(cli_addr);

                // This accept() function will write the connecting client's address info 
                // into the the address structure and the size of that structure is clilen.
                // The accept() returns a new socket file descriptor for the accepted connection.
                // So, the original socket file descriptor can continue to be used 
                // for accepting new connections while the new socker file descriptor is used for
                // communicating with the connected client.
                while(1){
                    newsockfd = accept(sockfd, 
                                (struct sockaddr *) &cli_addr, &clilen);
                    if (newsockfd < 0) {
                        std::cerr << "ERROR on accept\n";
                        exit(ACCEPT_ERROR);
                    }

                    std::cout << "Sending message...\n";
                    // This send() function sends the 47 bytes of the string to the new socket
                    send(newsockfd, "This is the server replying. Not HTTP of course", 47, 0);

                    close(newsockfd);
                }
                close(sockfd);
            }

        };

}