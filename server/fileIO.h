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
Post deserialize(const string &data);

// Saves a vector of Post data into a file
void savePoststoFile(vector<Post>&posts, const string& filename);

// Reads all post data from file and converts to Posts
vector<Post> loadPostsFromFile(const string& filename);