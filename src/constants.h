/**
* @file constants.h
* @brief Declarations of system wide constants
*/

#ifndef REMOTE_TOP_CONSTANTS_H

#    include <cstdint>

constexpr uint16_t SERVER_PORT = 5001U; /**< Fixed server port */

/**@{
 * @brief Suppress not used var warning
 * @param v value to void
 */
template <typename T> inline void NOT_USED_VAR( T v ) { static_cast<void>( v ); }
/**@}*/

#endif //REMOTE_TOP_CONSTANTS_H
