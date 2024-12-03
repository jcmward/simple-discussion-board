/*
 * CSCN72020: Computer Networks
 * Author: Max Venables
 * 
 * The server program's data input parsing functions.
 */

#include "dataParse.h"

int extractId(const string &input, string &remainingData){
    istringstream stream(input);
    string idSegment;
    
    // Ensuring an id is actually included
    if(!getline(stream, idSegment, RECORD_DELIMITER)){
        cout << "ERROR: cannot find data ID" << endl;
        return -1;
    }

    remainingData = input.substr(input.find(RECORD_DELIMITER) + 1); // only includes data after the id
    return stoi(idSegment);
}

vector<string> splitFields(const string &record){
    vector<string> fields;
    istringstream stream(record);
    string field;

    while(getline(stream, field, FIELD_DELIMITER)){
        fields.push_back(field);
    }
    return fields;
}

vector<Post> parsePostData(const string &data){
    vector<Post> posts;
    istringstream stream(data);
    string segment;

    while(getline(stream, segment, RECORD_DELIMITER)){ // Going through each post
        auto fields = splitFields(segment);

        string author = fields[0];
        string topic = fields[1];
        string body = fields[2];

        posts.emplace_back(author, topic, body);
    }

    return posts;
}

string postsToString(vector<Post>&posts){
    string allPosts;
    for (size_t i = 0; i < posts.size(); ++i) {
        if (i > 0) { // Only posts after the first get a record delimiter at the beginning
            allPosts.push_back(RECORD_DELIMITER);
        }
        allPosts.append(posts[i].serialize());
    }
    return allPosts;
}