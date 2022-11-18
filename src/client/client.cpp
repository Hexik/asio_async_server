#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "constants.h"

int main( int argc, char** argv )
{
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
            auto s = std::string( argv[i] ) + "\n";
            boost::asio::write( socket, boost::asio::buffer( s ), error );
            if( error ) {
                std::cerr << "send failed: " << error.message() << std::endl;
            }
            // getting response from server
            boost::asio::streambuf receive_buffer;
            boost::asio::read( socket, receive_buffer, boost::asio::transfer_at_least( 1 ), error );
            if( error && error != boost::asio::error::eof ) {
                std::cerr << "receive failed: " << error.message() << std::endl;
            } else {
                const char* data = boost::asio::buffer_cast<const char*>( receive_buffer.data() );
                std::cout << data;
            }
        }
    } catch( std::exception& e ) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
    return 0;
}
