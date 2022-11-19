#include "AsyncServer.h"
#include "utilities.h"
#include "constants.h"
#include <iostream>

AsyncServer::AsyncServer( boost::asio::io_context& context, uint16_t port ) :
    m_acceptor { context, { boost::asio::ip::tcp::v4(), port } }, task( context )
{
    NOT_USED_VAR( getCPU() ); // dirty hack: call this function to initialize prevXXX variables for the first time
    async_accept_one();
}

void AsyncServer::report_error( const std::string& component, boost::system::error_code ec )
{
    std::cerr << component << " failure: " << ec << " ()" << ec.message() << ")\n";
}

void AsyncServer::session( boost::asio::ip::tcp::socket socket )
{
    struct session_state {
        boost::asio::ip::tcp::socket m_socket;
        boost::asio::streambuf       m_streambuf;

        explicit session_state( boost::asio::ip::tcp::socket s ) : m_socket( std::move( s ) ) { }

        static void do_read( const std::shared_ptr<session_state>& state )
        {
            std::cerr << "Session read " << std::endl;

            boost::asio::async_read_until( state->m_socket, state->m_streambuf, "\n",
                                           [state]( const boost::system::error_code& ec, std::size_t length ) {
                                               if( !ec ) {
                                                   assert( state->m_streambuf.size() >= length );

                                                   do_write( state, length );
                                               } else if( ec == boost::asio::error::eof ) {
                                                   std::cerr << "Session done " << std::endl;
                                               } else {
                                                   report_error( "Session", ec );
                                               }
                                           } );
        }

        static void do_write( const std::shared_ptr<session_state>& state, std::size_t length )
        {
            std::cerr << "Session write " << std::endl;

            auto command =
                Trim( std::string( boost::asio::buffer_cast<const char*>( state->m_streambuf.data() ), length - 1 ) );
            command += "=" + getResponse( command ) + "\n";

            state->m_streambuf.consume( length );

            boost::asio::async_write( state->m_socket, boost::asio::buffer( command ),
                                      [state]( boost::system::error_code ec, std::size_t /*length*/ ) {
                                          if( !ec ) {
                                              do_read( state );
                                          } else {
                                              report_error( "Session", ec );
                                          }
                                      } );
        }
    };

    std::cerr << "Session started " << std::endl;
    auto s = std::make_shared<session_state>( std::move( socket ) );
    session_state::do_read( s );
}

void AsyncServer::async_accept_one()
{
    m_acceptor.async_accept( [this]( boost::system::error_code ec, boost::asio::ip::tcp::socket socket ) {
        if( !ec ) {
            session( std::move( socket ) );
        } else {
            report_error( "Listener", ec );
        }

        async_accept_one();
    } );
}
