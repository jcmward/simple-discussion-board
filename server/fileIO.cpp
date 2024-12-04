/*
 * CSCN72020: Computer Networks
 * Author: Max Venables
 * 
 * Server program's file IO functionality.
 */

#include "fileIO.h"

Post deserializePost(const string &data){
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
            posts.push_back(deserializePost(line));
        }
    }
    
    file.close();
    cout << "Posts read from file successfully." << endl;
    return posts;
}

Account deserializeAccount(const string &data){
    istringstream stream(data);
    string username, password;

    getline(stream, username, FIELD_DELIMITER);
    getline(stream, password, FIELD_DELIMITER);

    return Account(username, password);
}

void saveAccountstoFile(vector<Account>&accounts, const string& filename){
    ofstream file(filename, ios::out | ios::trunc);
    
    if(!file){
        cout << "ERROR: could not open file" << endl;
        return;
    }
    
    for (const auto& account : accounts){
        file << account.serialize() << "\n";
    }
    file.close();
    cout << "Accounts saved to file successfully." << endl;
}

vector<Account> loadAccountsFromFile(const string& filename){
    vector<Account> accounts;
    ifstream file(filename, ios::in);

    if(!file){
        cout << "No accounts found on file." << endl;
        return accounts;
    }

    string line;
    while(getline(file, line)){
        if(!line.empty()){
            accounts.push_back(deserializeAccount(line));
        }
    }
    
    file.close();
    cout << "Accounts read from file successfully." << endl;
    return accounts;
}