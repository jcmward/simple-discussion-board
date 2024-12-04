#pragma once
/*
 * CSCN72020: Computer Networks
 * Author: Max Venables
 * 
 * Header for the server program's data input parsing.
 */

#include "../common.h"

#include <sstream>
#include <iostream>

using namespace std;

// Looks for the id at the beginning of the input data and returns that
int extractId(const string &input, string &remainingData);

// Splits the given data into fields according to the delimiter and returns them
vector<string> splitFields(const string &record);

// Creates a vector of the input post data
vector<Post> parsePostData(const string &data);

// Converts a vector of Posts into a string
string postsToString(vector<Post>&posts);

// TODO: figure out how to combine all the "filterBy" functions, if possible

// Creates a new vector of Posts filtered by a specific topic
vector<Post> filterByTopic(const string &filter, vector<Post>&posts);

// Creates a new vector of Posts filtered by a specific author
vector<Post> filterByAuthor(const string &filter, vector<Post>&posts);

// Creates a new vector of Posts filtered by a specific keyword
vector<Post> filterByKeyword(const string &filter, vector<Post>&posts);