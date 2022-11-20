#include <boost/asio.hpp>
#include "server/PeriodicTask.h"
#include "utilities.h"
#include <iostream>
#include <mutex>

class AsyncServer
{
public:
    AsyncServer( boost::asio::io_context& context, uint16_t port, bool verbose );

private:
    struct session_state {
        boost::asio::ip::tcp::socket m_socket;
        boost::asio::streambuf       m_streambuf;

        explicit session_state( boost::asio::ip::tcp::socket s ) : m_socket( std::move( s ) ) { }
    };

    void do_read( const std::shared_ptr<session_state>& state );
    void do_write( const std::shared_ptr<session_state>& state, std::size_t length );

    static void report_error( const std::string& component, boost::system::error_code ec );
    void        session( boost::asio::ip::tcp::socket socket );
    void        async_accept_one();
    std::string getResponse( const std::string& cmd );

    boost::asio::ip::tcp::acceptor m_acceptor;
    PeriodicTask                   task;
    const bool                     m_verbose;
    std::string                    m_CPU;
    std::mutex                     m_mutex;
};
