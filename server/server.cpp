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
#include <fstream>

const int PORT_NUM = 8080; // might need to change this later

int main(){
    string filename = "posts.txt";
    string returnData;

    vector<Post> posts = loadPostsFromFile(filename);

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
    cout << "Listening on port: " << PORT_NUM << endl;

    // Accept
    int ConnectionSocket;
    ConnectionSocket = -1;
    if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == -1){
        close(ServerSocket);
        return 0;
    }
    cout << "Connection from client accepted!" << endl;

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
        close(ConnectionSocket);
        close(ServerSocket);
        return 0;
    }
    string fullMessage = string(message.begin(), message.end());

    // Check received data type
    string remainingData;
    int dataType = extractId(fullMessage, remainingData);

    cout << "Recieved data of type: " << dataType << endl;

    switch(dataType){
        case POSTS:
            posts = parsePostData(remainingData);
            break;
        case REQUEST_DISCUSSION:
            returnData = postsToString(posts);
            break;
        default:
            cout << "ERROR: invalid data type" << endl;
            break;
    }
    
    // Send requested data
    if (send(ConnectionSocket, returnData.c_str(), sizeof(returnData), 0) == -1){
        cout << "ERROR: data send failed" << endl;
        close(ConnectionSocket);
        close(ServerSocket);
        return 0;
    }

    // Save posts before shutdown
    savePoststoFile(posts, filename);

    // Cleanup
    close(ConnectionSocket);
    close(ServerSocket);
    return 0;
}