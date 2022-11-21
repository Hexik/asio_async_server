/**
* @file PeriodicTask.cpp
* @brief Implementation of PeriodicTask class
*/

#include "PeriodicTask.h"
#include "constants.h"

PeriodicTask::PeriodicTask( boost::asio::io_context& context ) : m_interval( 0 ), m_timer( context ) { }

void PeriodicTask::start( Duration interval, Callback cb )
{
    m_interval = interval;
    m_callback = std::move( cb );
    ENSURES( m_callback != nullptr );
    run();
}

void PeriodicTask::run()
{
    m_timer.expires_from_now( m_interval );
    m_timer.async_wait( [=]( boost::system::error_code ec ) {
        if( !ec ) {
            m_callback();
            run();
        }
    } );
}
