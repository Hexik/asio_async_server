#include "server/AsyncServer.h"

int main()
{
    try {
        boost::asio::io_context context;

        boost::asio::signal_set signals( context, SIGINT, SIGTERM );
        signals.async_wait( [&]( boost::system::error_code /*ec*/, int /*signo*/ ) { context.stop(); } );

        AsyncServer listen( context, 5001 );

        context.run();
        std::cerr << "Server done \n";
    } catch( std::exception& e ) {
        std::cerr << "Server failure: " << e.what() << "\n";
    }
}
