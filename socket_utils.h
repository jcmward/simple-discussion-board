// Socket management

#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <string>

int connect_socket(const char* server_address, const unsigned short server_port);

void send_data(int socket, const std::string &data);

void close_socket(int socket);

#endif

