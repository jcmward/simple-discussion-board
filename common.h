/*
 * CSCN72020: Computer Networks
 * Authors: Jon Ward, Max Venables, Tanishk Sharma
 *
 * Common header file for client and server
 */

#include <string>
#include <vector>

const char RECORD_DELIMITER = '|';
const char FIELD_DELIMITER = ':';

typedef enum message_type {
    POSTS               = 10,
    SIGNUP              = 20,
    LOGIN               = 21,
    // LOGOUT              = 22,    // maybe later
    // DELETE_ACCOUNT      = 23,    // maybe later
    REQUEST_DISCUSSION  = 30,
    REQUEST_TOPIC       = 31,
    REQUEST_AUTHOR      = 32,
    REQUEST_POST        = 33,
    RETURN_CODE         = 40,
} MESSAGE_TYPE;

class Account {
    private:
        std::string username;
        std::string password;
    public:
        // methods
};

class Post {
    private:
        std::string author;
        std::string topic;
        std::string text;
    public:
        // methods
};

class Topic {
    private:
        std::string author;
        std::string title;
        std::vector<Post> posts;
    public:
        // methods
};

