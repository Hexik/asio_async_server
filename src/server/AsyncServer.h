/**
* @file AsyncServer.h
* @brief Declaration of class encapsulating TCP server functionality
*/

#include <boost/asio.hpp>
#include "server/PeriodicTask.h"
#include "utilities.h"
#include <mutex>

/**
 * @class AsyncServer
 * @brief Server as an object
 */
class AsyncServer
{
public:
    /**
     * @brief Constructor
     * @param context asio service
     * @param port server port
     * @param verbose output log data on/off
     */
    AsyncServer( boost::asio::io_context& context, uint16_t port, bool verbose );

private:
    /**
     * @struct session_state
     * @brief Session encapsulation
     */
    struct session_state {
        boost::asio::ip::tcp::socket m_socket;    /**< boost TCP socket */
        boost::asio::streambuf       m_streambuf; /**< reader data */

        /**
         * @brief Constructor
         * @param socket boost TCP socket
         */
        explicit session_state( boost::asio::ip::tcp::socket socket ) : m_socket( std::move( socket ) ) { }
    };

    /**
     * @brief Read from client
     * @param state connection data
     */
    void do_read( const std::shared_ptr<session_state>& state );

    /**
     * @brief Write response to the client
     * @param state connection data
     * @param length data size
     */
    void do_write( const std::shared_ptr<session_state>& state, std::size_t length );

    /**
     * @brief Outputs error status
     * @param component error source
     * @param ec error code
     */
    static void report_error( const std::string& component, boost::system::error_code ec );

    /**
     * @brief Session with one connected client
     * @param socket boost TCP socket
     */
    void session( boost::asio::ip::tcp::socket socket );

    /**
     * @brief Prepare m_acceptor for incoming connection
     */
    void async_accept_one();

    /**
     * @brief Returns response to command/request
     * @param req request
     * @return response to the request, "NA" if wrong request
     */
    std::string getResponse( const std::string& req ) attr_wur;

    boost::asio::ip::tcp::acceptor m_acceptor; /**< accepts incoming TCP connections */
    PeriodicTask                   m_task;     /**< periodic task */
    const bool                     m_verbose;  /**< verbosity level */
    std::string                    m_CPU;      /**< precalculated answer to 'cpu' command, made by PeriodicTask */
    std::mutex                     m_mutex;    /**< lock shared data */
};
