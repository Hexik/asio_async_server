#ifndef REMOTE_TOP_PERIODICTASK_H
#define REMOTE_TOP_PERIODICTASK_H

#include <boost/asio.hpp>

using Duration = std::chrono::high_resolution_clock::duration;
using Callback = std::function<void()>;

/**
 * @struct PeriodicTask
 * @brief  simple wrapper that makes it easier to repeat action on fixed intervals
 */
struct PeriodicTask {
    /**
     * @brief Constructor
     * @param context asio service
     */
    explicit PeriodicTask( boost::asio::io_context& context );

    /**
     * @brief Set data and start task
     * @param interval period length
     * @param cb worker function
     */
    void start( Duration interval, Callback cb );

private:
    /**
     * @brief Called at the end of every period
     */
    void run();

    Duration                           m_interval;           /**< period between run() invocation */
    Callback                           m_callback = nullptr; /**< worker function */
    boost::asio::high_resolution_timer m_timer;              /**< time measurement */
};
#endif //REMOTE_TOP_PERIODICTASK_H
