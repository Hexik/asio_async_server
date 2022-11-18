#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

class AsyncServer
{
public:
    AsyncServer( boost::asio::io_context& context, uint16_t port );

private:
    static void report_error( const std::string& component, boost::system::error_code ec );
    static void session( boost::asio::ip::tcp::socket socket );
    void        async_accept_one();

    boost::asio::ip::tcp::acceptor m_acceptor;
};
