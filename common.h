/*
 * CSCN72020: Computer Networks
 * Authors: Jon Ward, Max Venables, Tanishk Sharma
 *
 * Common header file for client and server
 */

#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const char* SERVER_ADDRESS = "172.16.5.12";
const unsigned short SERVER_PORT = 8080;
const char* CLIENT_ADDRESS = "172.16.5.50";
const unsigned short CLIENT_PORT = 8080;

const char RECORD_DELIMITER = '|';
const char FIELD_DELIMITER = ':';

const int MAX_PACKET = 1500;

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

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        tokens.emplace_back(item);
    }
    return tokens;
}

class Account {
private:
    std::string username;
    std::string password;

public:
    Account() {
        username = "";
        password = "";
    }

    Account(std::string u, std::string p) {
        username = u;
        password = p;
    }

    std::string get_username() const {
        return username;
    }

    std::string serialize() const {
        return username + FIELD_DELIMITER + password;
    }

    bool deserialize(const std::string& data) {
        std::vector<std::string> fields = split(data, FIELD_DELIMITER);
        if (fields.size() != 2) {
            return false;
        }
        username = fields[0];
        password = fields[1];
        return true;
    }
};

class Post {
private:
    std::string author;
    std::string topic;
    std::string text;

public:
    Post(){
        author = "";
        topic = "";
        text = "";
    }

    Post(std::string a, std::string b, std::string c){
        author = a;
        topic = b;
        text = c;
    }

    std::string get_author() {
        return author;
    }

    std::string get_topic() {
        return topic;
    }

    std::string get_text() {
        return text;
    }


    std::string serialize() const {
        return author + FIELD_DELIMITER + topic + FIELD_DELIMITER + text;
    }

    bool deserialize(const std::string& data) {
        std::vector<std::string> fields = split(data, FIELD_DELIMITER);
        if (fields.size() != 3) {
            return false;
        }
        author = fields[0];
        topic = fields[1];
        text = fields[2];
        return true;
    }

    void display() {
        std::cout << "Author: " << (author.empty() ? "[none]" : author) << std::endl;
        std::cout << "Topic: " << (topic.empty() ? "[none]" : topic) << std::endl;
        std::cout << "Text: " << (text.empty() ? "[none]" : text) << std::endl;
    }
};

class Topic {
private:
    std::string author;
    std::string title;
    std::vector<Post> posts;

public:
    // methods
};

#endif

