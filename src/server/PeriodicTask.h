#ifndef REMOTE_TOP_PERIODICTASK_H
#define REMOTE_TOP_PERIODICTASK_H

#include <boost/asio.hpp>

using Clock    = std::chrono::high_resolution_clock;
using Callback = std::function<void()>;

// simple wrapper that makes it easier to repeat on fixed intervals
struct PeriodicTask {
    PeriodicTask( boost::asio::io_context& context );
    void start( Clock::duration interval, Callback cb );

private:
    void run();

    Clock::duration                    m_interval;
    Callback                           m_callback;
    boost::asio::high_resolution_timer m_timer;
};
#endif //REMOTE_TOP_PERIODICTASK_H
