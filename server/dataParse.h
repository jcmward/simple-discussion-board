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