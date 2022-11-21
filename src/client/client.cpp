/**
* @file client.cpp
* @brief Simple TCP client
*/

#include <boost/asio.hpp>
#include "constants.h"

/**
 * @brief Client - Starting point
 * @param argc arg count
 * @param argv arguments
 * @return status
 */
int main( int argc, char** argv )
{
    if( argc < 2 ) {
        std::cerr << "Usage: client IP_ADDRESS [mem|cpu]" << std::endl;
        return 0;
    }

    try {
        boost::asio::io_service io_service;
        //socket creation
        boost::asio::ip::tcp::socket socket( io_service );
        //connection
        socket.connect(
            boost::asio::ip::tcp::endpoint( boost::asio::ip::address::from_string( argv[1] ), SERVER_PORT ) );

        // request/message from client
        boost::system::error_code error;
        for( int i = 2; i < argc; ++i ) {
            const auto s = std::string( argv[i] ) + "\n";
            boost::asio::write( socket, boost::asio::buffer( s ), error );
            if( error ) {
                std::cerr << "send failed: " << error.message() << std::endl;
            }

            // getting response from server
            boost::asio::streambuf receive_buffer;

            // read one line from socket
            boost::asio::async_read_until(
                socket, receive_buffer, "\n", [&]( const boost::system::error_code& ec, std::size_t length ) {
                    if( !ec ) {
                        EXPECTS( receive_buffer.size() >= length );

                        const char* data = boost::asio::buffer_cast<const char*>( receive_buffer.data() );
                        std::cout << data;

                    } else if( ec == boost::asio::error::eof ) {
                        std::cerr << "Session done " << std::endl;
                    } else {
                        std::cerr << "Session Error " << std::endl;
                    }
                } );

            io_service.run_one();
            io_service.reset();

            // short delay between requests, should be commented out
            std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
        }
    } catch( std::exception& e ) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
