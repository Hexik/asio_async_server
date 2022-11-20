/**
* @file constants.h
* @brief Declarations of system wide constants
*/

#ifndef REMOTE_TOP_CONSTANTS_H
#define REMOTE_TOP_CONSTANTS_H

#include <cstdint>

constexpr uint16_t SERVER_PORT = 5001U; /**< Fixed server port */

#ifdef DEBUG
constexpr bool VERBOSITY = true;
#else
constexpr bool VERBOSITY = false;
#endif

/**@{
 * @brief Design by contract macros
 * @param cond assert condition
 */
#ifdef DEBUG
#    define EXPECTS( cond )                                       \
        do {                                                      \
            if( !( cond ) ) {                                     \
                std::cerr << "Expects: %s" << #cond << std::endl; \
                assert( cond );                                   \
            }                                                     \
        } while( false )
#    define ENSURES( cond )                                       \
        do {                                                      \
            if( !( cond ) ) {                                     \
                std::cerr << "Expects: %s" << #cond << std::endl; \
                assert( cond );                                   \
            }                                                     \
        } while( false )
#else
#    define EXPECTS( cond )       \
        do {                      \
            NOT_USED_VAR( cond ); \
        } while( false )
#    define ENSURES( cond )       \
        do {                      \
            NOT_USED_VAR( cond ); \
        } while( false )
#endif

/**@{
 * @brief Suppress not used var warning
 * @param v value to void
 */
template <typename T> inline void NOT_USED_VAR( T v ) { static_cast<void>( v ); }
/**@}*/

static_assert( static_cast<char>( 0x80U ) > 0U, "char should be set as unsigned" );

/**@{*/
/** Functions attributes */
#define attr_const __attribute__( ( const ) )
#define attr_pure __attribute__( ( pure ) )
#define attr_unused __attribute__( ( unused ) )
#define attr_wur __attribute__( ( warn_unused_result ) )
/**@}*/

#endif //REMOTE_TOP_CONSTANTS_H
