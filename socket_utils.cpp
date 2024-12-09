// Socket management

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "socket_utils.h"

int connect_socket(const char* server_address, const unsigned short server_port) {
    // Create socket to contact server
    int socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket < 0) {
        std::cerr << "ERROR: Failed to create client socket" << std::endl;
        return -1;
    }

    // Configure server address and port
    sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_addr.s_addr = inet_addr(server_address);
    server_sockaddr.sin_port = htons(server_port);

    // Initiate three-way handshake with server
    if ((connect(socket, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr))) < 0) {
        close_socket(socket);
        std::cerr << "ERROR: Connection attempted failed" << std::endl;
        return -1;
    }

    return socket;
}

void close_socket(int socket) {
    close(socket);
}

