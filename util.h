#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <string>
#include <vector>
#include "tweet.h"

// Converts a string to all lower case
void convLower(std::string& s);

// Converts a string to all upper case
void convUpper(std::string& s);

// Display a  list of tweets to stdout (cout)
void displayHits(std::vector<Tweet*>& hits);

// Used from http://stackoverflow.com/questions
///216823/whats-the-best-way-to-trim-stdstring
// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), 
        	std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), 
        	std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

#endif
