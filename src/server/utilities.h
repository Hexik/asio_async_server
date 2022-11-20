#ifndef REMOTE_TOP_UTILITIES_H
#define REMOTE_TOP_UTILITIES_H

#include "constants.h"
#include <string>

std::string Trim( const std::string& str ) attr_wur;

std::string getCPU() attr_wur;
std::string getMEM() attr_wur;

#endif //REMOTE_TOP_UTILITIES_H
