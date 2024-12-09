// Socket management

#include <string>

int connect_socket(const char* server_address, const unsigned short server_port);

void send_data(int socket, std::string data);

void close_socket(int socket);

