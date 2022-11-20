#include "AsyncServer.h"
#include "utilities.h"

AsyncServer::AsyncServer( boost::asio::io_context& context, uint16_t port, bool verbose ) :
    m_acceptor { context, { boost::asio::ip::tcp::v4(), port } }, task( context ), m_verbose( verbose )
{
    NOT_USED_VAR( getCPU() ); // dirty hack: call this function to initialize prevXXX variables for the first time
    task.start( std::chrono::milliseconds( 500 ), [this]() {
        auto lock = std::unique_lock<std::mutex>( m_mutex );
        m_CPU = getCPU();
    } );
    async_accept_one();
}

void AsyncServer::report_error( const std::string& component, boost::system::error_code ec )
{
    std::cerr << component << " failure: " << ec << " ()" << ec.message() << ")\n";
}

void AsyncServer::session( boost::asio::ip::tcp::socket socket )
{
    if( m_verbose ) {
        std::cerr << "Session started " << std::endl;
    }
    auto s = std::make_shared<session_state>( std::move( socket ) );
    do_read( s );
}

std::string AsyncServer::getResponse( const std::string& cmd )
{
    if( cmd == "cpu" ) {
        auto lock = std::unique_lock<std::mutex>( m_mutex );
        return m_CPU;
    }
    if( cmd == "mem" ) {
        return getMEM();
    }
    return "NA";
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

void AsyncServer::do_read( const std::shared_ptr<session_state>& state )
{
    boost::asio::async_read_until( state->m_socket, state->m_streambuf, "\n",
                                   [state, this]( const boost::system::error_code& ec, std::size_t length ) {
                                       if( !ec ) {
                                           assert( state->m_streambuf.size() >= length );

                                           do_write( state, length );
                                       } else if( ec == boost::asio::error::eof ) {
                                           if( m_verbose ) {
                                               std::cerr << "Session done " << std::endl;
                                           }
                                       } else {
                                           report_error( "Session", ec );
                                       }
                                   } );
}

void AsyncServer::do_write( const std::shared_ptr<session_state>& state, std::size_t length )
{
    auto command =
        Trim( std::string( boost::asio::buffer_cast<const char*>( state->m_streambuf.data() ), length - 1 ) );
    if( m_verbose ) {
        std::cerr << "Session read " << command << std::endl;
    }

    command += "=" + getResponse( command ) + "\n";
    if( m_verbose ) {
        std::cerr << "Session write " << command;
    }

    state->m_streambuf.consume( length );

    boost::asio::async_write( state->m_socket, boost::asio::buffer( command ),
                              [state, this]( boost::system::error_code ec, std::size_t /*length*/ ) {
                                  if( !ec ) {
                                      do_read( state );
                                  } else {
                                      report_error( "Session", ec );
                                  }
                              } );
}
