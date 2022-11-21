/**
 * @file main.cpp
 * @brief Main program
 */

#include <sys/syslog.h>
#include "main.h"
#include "server/AsyncServer.h"

#include "constants.h"

#include "git_rev.h"

/**
 * @brief Starting point, implementation used here was found on Internet,
 * I guess that original version is here
 * https://www.boost.org/doc/libs/1_73_0/doc/html/boost_asio/example/cpp11/fork/daemon.cpp
 * @return non-zero if error
 */
int main( int /*argc*/, char** /*argv*/ )
{
    try {
        boost::asio::io_context io_context;

        // Initialise the server
        AsyncServer server( io_context, SERVER_PORT, VERBOSITY );

        // Signal handlers
        boost::asio::signal_set signals( io_context, SIGINT, SIGTERM );
        signals.async_wait( [&]( boost::system::error_code /*ec*/, int /*signo*/ ) { io_context.stop(); } );

        io_context.notify_fork( boost::asio::io_context::fork_prepare );

        // Fork the process and let the parent exit
        if( pid_t pid = fork() ) {
            if( pid > 0 ) {
                exit( 0 );
            } else {
                syslog( LOG_ERR | LOG_USER, "First fork failed" );
                return 1;
            }
        }

        setsid();

        NOT_USED_VAR( chdir( "/" ) );

        umask( 0 );

        // A second fork ensures the process cannot acquire a controlling terminal.
        if( pid_t pid = fork() ) {
            if( pid > 0 ) {
                exit( 0 );
            } else {
                syslog( LOG_ERR | LOG_USER, "Second fork failed" );
                return 1;
            }
        }

        // Close the standard streams.
        close( 0 );
        close( 1 );
        close( 2 );

        if( open( "/dev/null", O_RDONLY ) < 0 ) {
            syslog( LOG_ERR | LOG_USER, "Unable to open /dev/null" );
            return 1;
        }

        // Send standard output to a log file.
        const char*  output = "/tmp/async.daemon.txt";
        const int    flags  = O_WRONLY | O_CREAT | O_APPEND;
        const mode_t mode   = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        if( open( output, flags, mode ) < 0 ) {
            syslog( LOG_ERR | LOG_USER, "Unable to open output file %s", output );
            return 1;
        }

        // Redirect standard error to the same log file.
        if( dup( 1 ) < 0 ) {
            syslog( LOG_ERR | LOG_USER, "Unable to dup output descriptor" );
            return 1;
        }

        // The io_context can now be used normally.
        io_context.notify_fork( boost::asio::io_context::fork_child );

        syslog( LOG_INFO | LOG_USER, "Daemon started %s", GIT_STR );
        io_context.run();
        syslog( LOG_INFO | LOG_USER, "Daemon stopped" );
    } catch( std::exception& e ) {
        syslog( LOG_ERR | LOG_USER, "Exception: %s", e.what() );
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
