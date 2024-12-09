// Socket management

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "socket_utils.h"

int connect_socket(const char* server_address, const unsigned short server_port) {
    // Create socket to contact server
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        std::cerr << "ERROR: Failed to create client socket" << std::endl;
        return -1;
    }

    // Configure server address and port
    sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_addr.s_addr = inet_addr(server_address);
    server_sockaddr.sin_port = htons(server_port);

    // Initiate three-way handshake with server
    if ((connect(sock, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr))) < 0) {
        close_socket(sock);
        std::cerr << "ERROR: Connection attempted failed" << std::endl;
        return -1;
    }

    return sock;
}

void send_data(int socket, const std::string &data) {
    std::cout << "Sending data: " << data << std::endl;

    int bytes_sent = send(socket, data.c_str(), data.size(), 0);
    if (bytes_sent < 0) {
        std::cerr << "ERROR: Failed to send data" << std::endl;
    } else {
        std::cout << "Sent: " << data << std::endl;
    }
}

void close_socket(int socket) {
    close(socket);
}

