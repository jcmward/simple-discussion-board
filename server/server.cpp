/*
 * CSCN72020: Computer Networks
 * Author: Max Venables
 * 
 * Server-side of the discussion board program.
 */

#include "dataParse.h"
#include "fileIO.h"
#include "../common.h"

#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

const int PORT_NUM = 8080; // might need to change this
bool running = true; // flag to determine when to shut down the server

/* 
 * Looks for a user input of "EXIT" as a command to shut down the server
 * I might alter this later if I can think of a better way to include proper shutting down of the server
 */
void inputThread(){
    string input;
    while(running){
        getline(cin, input);
        if (input == "EXIT"){
            cout << "Shutdown command recieved. Stopping server..." << endl;
            running = false;
        }
    }
}

int main(){
    string postFile = "posts.txt";
    string accFile = "accounts.txt";
    string returnData;

    vector<Post> posts = loadPostsFromFile(postFile);
    vector<Account> accounts = loadAccountsFromFile(accFile);

    // Socket
    int ServerSocket;
    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ServerSocket == -1){
        cout << "ERROR: Failed to create ServerSocket" << std::endl;
        return 0;
    }
    cout << "Server Socket created." << endl;

    // Bind
    sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_addr.s_addr = INADDR_ANY;
    SvrAddr.sin_port = htons(PORT_NUM);
    if (bind(ServerSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr)) == -1){
        close(ServerSocket);
        cout << "ERROR: Failed to bind ServerSocket" << endl;
        return 0;
    }
    cout << "Server Socket bound." << endl;

    // Listen
    if (listen(ServerSocket, 1) == -1){
        close(ServerSocket);
        cout << "ERROR: listen failed to configure ServerSocket" << endl;
        return 0;
    }
    cout << "Server is listening on port: " << PORT_NUM << endl;

    // Accept
    int ConnectionSocket;
    ConnectionSocket = -1;
    if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == -1){
        cout << "ERROR accepting client connection" << endl;
        close(ServerSocket);
        return 0;
    }
    cout << "Connection from client accepted!" << endl;

    // detecting user input to shut down server
    thread shutdownThread(inputThread);

    // Loops through the message recieving and sending until told to shut down
    while(running){
        // Ensures all data is recieved even if it goes over the buffer size
        char buffer[4906] = {0};
        vector<char> message;
        ssize_t bytesRead;

        while((bytesRead = recv(ConnectionSocket, buffer, sizeof(buffer), 0)) > 0){
            message.insert(message.end(), buffer, buffer + bytesRead);

            if (message.size() >= sizeof(buffer) || strstr(buffer, "\n") != nullptr){
                break;
            }
        }
        if(bytesRead == -1){
            cout << "ERROR: data not received correctly" << endl;
            break;
        }
        string fullMessage = string(message.begin(), message.end());

        // Check received data type
        string remainingData;
        int dataType = extractId(fullMessage, remainingData);

        cout << "Recieved data of type: " << dataType << endl;

        /* 
         * I wasn't sure if I should use return codes when sending info or if the client used the same codes as the server
         * So that stuff might be changed later
         */
        vector<Post> tempPosts;
        Account tempAcc;
        switch(dataType){
            case POSTS:
                tempPosts = parsePostData(remainingData);
                posts.insert(posts.end(), tempPosts.begin(), tempPosts.end());
                returnData = POSTS + "|";
                break;
            case SIGNUP:
                tempAcc = parseAccountData(remainingData);
                accounts.push_back(tempAcc);
                returnData = SIGNUP + "|";
                break;
            case LOGIN:
                tempAcc = parseAccountData(remainingData);
                returnData = LOGIN + "|";
                returnData.append(to_string(isValidAccount(tempAcc, accounts)));
                break;
            case REQUEST_DISCUSSION:
                returnData = REQUEST_DISCUSSION + "|";
                returnData.append(postsToString(posts));
                break;
            case REQUEST_TOPIC:
                tempPosts = filterByTopic(remainingData, posts);
                returnData = REQUEST_TOPIC + "|";
                returnData.append(postsToString(tempPosts));
                break;
            case REQUEST_AUTHOR:
                tempPosts = filterByAuthor(remainingData, posts);
                returnData = REQUEST_AUTHOR + "|";
                returnData.append(postsToString(tempPosts));
                break;
            case REQUEST_POST:
                tempPosts = filterByKeyword(remainingData, posts);
                returnData = REQUEST_POST + "|";
                returnData.append(postsToString(tempPosts));
                break;
            default:
                cout << "ERROR: invalid data type" << endl;
                returnData = RETURN_CODE + "|";
                break;
        }
        
        // Send requested data
        if (send(ConnectionSocket, returnData.c_str(), sizeof(returnData), 0) == -1){
            cout << "ERROR: data send failed" << endl;
            break;
        }
    }

    // Save posts before shutdown
    savePoststoFile(posts, postFile);

    // Cleanup
    close(ConnectionSocket);
    close(ServerSocket);
    return 0;
}