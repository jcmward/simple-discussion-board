/*
 * CSCN72020: Computer Networks
 * Author: Max Venables
 * 
 * Server program's file IO functionality.
 */

#include "fileIO.h"

Post deserialize(const string &data){
    istringstream stream(data);
    string author, topic, text;

    getline(stream, author, FIELD_DELIMITER);
    getline(stream, topic, FIELD_DELIMITER);
    getline(stream, text, FIELD_DELIMITER);

    return Post(author, topic, text);
}

void savePoststoFile(vector<Post>&posts, const string& filename){
    ofstream file(filename, ios::out | ios::trunc);
    
    if(!file){
        cout << "ERROR: could not open file" << endl;
        return;
    }
    
    for (const auto& post : posts){
        file << post.serialize() << "\n";
    }
    file.close();
    cout << "Posts saved to file successfully." << endl;
}

vector<Post> loadPostsFromFile(const string& filename){
    vector<Post> posts;
    ifstream file(filename, ios::in);
    
    if(!file){
        cout << "No posts found on file." << endl;
        return posts;
    }

    string line;
    while(getline(file, line)){
        if(!line.empty()){
            posts.push_back(deserialize(line));
        }
    }
    
    file.close();
    cout << "Posts read from file successfully." << endl;
    return posts;
}