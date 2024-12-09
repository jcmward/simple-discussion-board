/*
 * CSCN72020: Computer Networks
 * Authors: Jon Ward, Max Venables, Tanishk Sharma
 *
 * Client for discussion board
 */

#include <iostream>
#include <sstream>
#include <sys/socket.h>

#include "common.h"
#include "socket_utils.h"

#define SERVER_ADDRESS  "127.0.0.1"
#define SERVER_PORT     27500

/* void handle_message(int msg_type) { */
/*     switch(msg_type) { */
/*         case RETURN_CODE: */

/*             break; */

/*         case POSTS: */

/*             break; */

/*         default: */
/*             std::cerr << "Received unexpected/unrecognized message type." << std::endl; */
/*             break; */
/*     } */
/* } */

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}

Post input_post() {
    std::string author, topic, text;
    std::cout << "Please provide the following details.  Author and topic can be left blank for an anonymous/generic post." << std::endl;
    std::cout << "Author: ";
    std::getline(std::cin, author);
    std::cout << "Topic: ";
    std::getline(std::cin, topic);

    do {
        std::cout << "Text: ";
        std::getline(std::cin, text);
        if (text.empty()) {
            std::cout << "Post text cannot be empty." << std::endl;
        }
    } while (text.empty());
    std::cout << std::endl;

    // TODO: input validation

    return Post(author, topic, text);
}

void send_post(Post post, int socket) {
    std::string serialized_post = std::to_string(static_cast<int>(POSTS));
    serialized_post += RECORD_DELIMITER;
    serialized_post += post.get_author();
    serialized_post += FIELD_DELIMITER;
    serialized_post += post.get_topic();
    serialized_post += FIELD_DELIMITER;
    serialized_post += post.get_text();

    send(socket, serialized_post.c_str(), serialized_post.size(), 0);
}

void send_multiple_posts(int socket) {
    // Get number of posts from user
    int num_posts = 0;
    do {
        std::cout << "How many posts would you like to send?" << std::endl;
        std::cin >> num_posts;
        std::cin.ignore();
        if (num_posts <= 0) {
            std::cout << "Number of posts must be greater than zero." << std::endl;
        }
    } while (num_posts <= 0);

    // Input and serialize posts
    std::ostringstream oss;
    Post post;
    for (int i = 0; i < num_posts; ++i) {
        std::cout << "Post " << i << "/" << num_posts << std::endl;
        post = input_post();
        oss << post.get_author() << FIELD_DELIMITER << post.get_topic() << FIELD_DELIMITER << post.get_text();
        if (i < num_posts - 1) {
            oss << RECORD_DELIMITER;
        }
    }
    std::string serialized_posts = std::to_string(static_cast<int>(POSTS));
    serialized_posts += RECORD_DELIMITER + oss.str();

    // Send serialized posts to server
    send(socket, serialized_posts.c_str(), serialized_posts.size(), 0);
}

void receive_posts(int socket) {
    char response[MAX_PACKET] = {0};
    int bytes_read = recv(socket, response, sizeof(response), 0);
    if (bytes_read <= 0) {
        std::cerr << "Failed to receive posts from server." << std::endl;
            return;
    }

    std::string response_str(response, bytes_read);
    std::vector<std::string> posts_data = split(response_str, RECORD_DELIMITER);

    for (const auto& post_data : posts_data) {
        Post post;
        if (post.deserialize(post_data)) {
            post.display();
        }
    }
}

void request_posts(int socket) {
    std::string message = std::to_string(static_cast<int>(REQUEST_DISCUSSION));
    send(socket, message.c_str(), message.size(), 0);

    receive_posts(socket);
}

void search_posts_by_author(int socket) {
    // Input search term
    std::string search_term;
    std::cout << "Author to search: ";
    std::getline(std::cin, search_term);

    // Serialize and send
    std::string message = std::to_string(static_cast<int>(REQUEST_AUTHOR));
    message += RECORD_DELIMITER + search_term;
    send(socket, message.c_str(), message.size(), 0);

    receive_posts(socket);
}

void search_posts_by_topic(int socket) {
    // Input search term
    std::string search_term;
    std::cout << "Topic to search: ";
    std::getline(std::cin, search_term);

    // Serialize and send
    std::string message = std::to_string(static_cast<int>(REQUEST_TOPIC));
    message += RECORD_DELIMITER + search_term;
    send(socket, message.c_str(), message.size(), 0);

    receive_posts(socket);
}

void login(int socket) {
    // Input username and password
    std::string username, password;
    std::cout << "Username: ";
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    // Serialize and send username and password
    std::string message = std::to_string(static_cast<int>(LOGIN));
    message += RECORD_DELIMITER;
    message += username;
    message += FIELD_DELIMITER;
    message += password;
    send(socket, message.c_str(), message.size(), 0);

    char response[MAX_PACKET] = {0};
    int bytes_read = recv(socket, response, sizeof(response), 0);
    if (bytes_read <= 0) {
        std::cerr << "Failed to receive authentication response from server." << std::endl;
    }

    // TODO: actually read and verify the return code
    std::cout << "Authentication succeeded." << std::endl;
}

// On the client side, signup logic is identical to login -- except for the message type
void signup(int socket) {
    // Input username and password
    std::string username, password;
    std::cout << "Username: ";
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    // Serialize and send username and password
    std::string message = std::to_string(static_cast<int>(SIGNUP));
    message += RECORD_DELIMITER;
    message += username;
    message += FIELD_DELIMITER;
    message += password;
    send(socket, message.c_str(), message.size(), 0);

    char response[MAX_PACKET] = {0};
    int bytes_read = recv(socket, response, sizeof(response), 0);
    if (bytes_read <= 0) {
        std::cerr << "Failed to receive authentication response from server." << std::endl;
    }

    // TODO: actually read and verify the return code
    std::cout << "Authentication succeeded." << std::endl;
}

bool run_menu(int socket) {
    std::cout << std::endl;
    std::cout << "Please enter one of the following numbers corresponding to a command." << std::endl;
    std::cout << "\t1. Add a single post" << std::endl;
    std::cout << "\t2. Add multiple posts" << std::endl;
    std::cout << "\t3. Request all posts from server" << std::endl;
    std::cout << "\t4. Search posts by author" << std::endl;
    std::cout << "\t5. Search posts by topic" << std::endl;
    std::cout << "\t6. Log in" << std::endl;
    std::cout << "\t7. Sign up" << std::endl;
    std::cout << "\t0. Exit Program" << std::endl;
    std::cout << std::endl;

    // TODO: input validation
    char user_response;
    std::cin >> user_response;
    switch (user_response) {
        case '1':
            send_post(input_post(), socket);
            break;

        case '2':
            send_multiple_posts(socket);
            break;

        case '3':
            request_posts(socket);
            break;

        case '4':
            search_posts_by_author(socket);
            break;

        case '5':
            search_posts_by_topic(socket);
            break;

        case '6':
            login(socket);
            break;

        case '7':
            signup(socket);
            break;

        case '0':
            // exit(EXIT_SUCCESS);
            return false;
            break;

        default:
            std::cout << "Invalid menu entry.  Please try again." << std::endl;
            break;
    }
    return true;    // Re-run the menu
}

int main () {
    int client_socket = connect_socket(SERVER_ADDRESS, SERVER_PORT);
    if (client_socket == -1) {
        exit(EXIT_FAILURE);
    }

    // Something like this
    const char* packet = "";
    int bytes_sent = send(client_socket, packet, MAX_PACKET, 0);
    if (bytes_sent < 0) {
        std::cerr << "ERROR: Failed to send data" << std::endl;
    } else {
        std::cout << "Sent: " << packet << std::endl;
    }

    /* std::vector<Post>* posts = { 0 }; */

    while (run_menu(client_socket)) {
        ;   // Nothing else needed here
    }

    close_socket(client_socket);
    return 0;
}

