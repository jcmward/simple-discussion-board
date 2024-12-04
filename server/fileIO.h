#pragma once
/*
 * CSCN72020: Computer Networks
 * Author: Max Venables
 * 
 * Header for the server program's file IO functionality.
 */

#include "../common.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Converts a string into a Post
Post deserializePost(const string &data);

// Saves a vector of Post data into a file
void savePoststoFile(vector<Post>&posts, const string& filename);

// Reads all post data from file and converts to Posts
vector<Post> loadPostsFromFile(const string& filename);

// Converts a string into an Account
Account deserializeAccount(const string &data);

// Saves a vector of Account data into a file
void saveAccountstoFile(vector<Account>&accounts, const string& filename);

// Reads all account data from a file and converts to Accounts
vector<Account> loadAccountsFromFile(const string& filename);