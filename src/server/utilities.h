//
// Created by mf on 18.11.22.
//

#ifndef REMOTE_TOP_UTILITIES_H
#define REMOTE_TOP_UTILITIES_H

#include <string>

std::string Trim( const std::string& str );

std::string getResponse( const std::string& cmd );

std::string getCPU();
std::string getMEM();

#endif //REMOTE_TOP_UTILITIES_H
