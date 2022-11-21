#ifndef REMOTE_TOP_UTILITIES_H
#define REMOTE_TOP_UTILITIES_H

#include "constants.h"
#include <string>

/**
*@brief Trim white chars from both end of the string
*@param str string to trim
*@return trimmed string
*/
std::string Trim( const std::string& str ) attr_wur;

/**
 * @brief Response to the 'cpu' command
 * @return formatted string
 */
std::string getCPU() attr_wur;

/**
* @brief Response to the 'mem' command
* @return formatted string
 */
std::string getMEM() attr_wur;

#endif //REMOTE_TOP_UTILITIES_H
