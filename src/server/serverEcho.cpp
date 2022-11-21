/**
* @file serverEcho.cpp
* @brief Server with console UI, no fork
*/

#include "server/AsyncServer.h"

/**
 * @brief AsyncServer basic usage
 */
int main( int /*argc*/, char** /*argv*/ )
{
    try {
        boost::asio::io_context context;

        boost::asio::signal_set signals( context, SIGINT, SIGTERM );
        signals.async_wait( [&]( boost::system::error_code /*ec*/, int /*signo*/ ) { context.stop(); } );

        AsyncServer server( context, SERVER_PORT, true );

        std::cerr << "Server start \n";
        context.run();
        std::cerr << "Server done \n";
    } catch( std::exception& e ) {
        std::cerr << "Server failure: " << e.what() << "\n";
    }
    return 0;
}
