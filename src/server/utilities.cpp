#include "utilities.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

std::string Trim( const std::string& str )
{
    const auto WhiteSpace = " \t\v\r\n";
    auto       tmp        = str;

    tmp.erase( 0, tmp.find_first_not_of( WhiteSpace ) );
    const auto idx = tmp.find_last_not_of( WhiteSpace );

    if( idx != std::string::npos ) {
        tmp.erase( idx + 1 );
    }
    return tmp;
}

std::string getCPU()
{
    std::ifstream ifs( "/proc/stat" );

    static int64_t prevTotal = 0;
    static int64_t prevIdle  = 0;

    if( ifs ) {
        std::string line;

        while( std::getline( ifs, line ) ) {
            std::istringstream iss( line );
            std::string        item;
            int64_t            user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

            if( iss >> item >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >>
                guest_nice ) {
                if( item == "cpu" ) {
                    // Calculation recipe from
                    // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux/
                    const auto sumIdle  = idle + iowait;
                    const auto NonIdle  = user + nice + system + irq + softirq + steal;
                    const auto sumTotal = sumIdle + NonIdle;

                    //differentiate : actual value minus the previous one
                    const auto totald = sumTotal - prevTotal;
                    const auto idled  = sumIdle - prevIdle;

                    const auto CPU_Percentage = ( totald == 0 ) ? 100.0f
                                                                : 100.0f * ( static_cast<float>( totald - idled ) ) /
                                                                      static_cast<float>( totald );

                    prevTotal = sumTotal;
                    prevIdle  = sumIdle;
                    std::stringstream ss;
                    ss << std::fixed << std::setprecision( 2 ) << CPU_Percentage << " %";
                    return ss.str();
                }
            } else {
                std::cerr << "/proc/stat has wrong format: " << line << std::endl;
                return "NaN";
            }
        }
    }
    return "NaN";

}

std::string getMEM()
{
    std::ifstream ifs( "/proc/meminfo" );

    if( ifs ) {
        std::string line;
        auto        counter = 0U;
        int64_t     result  = 0;

        while( std::getline( ifs, line ) && counter < 4U ) {
            std::istringstream iss( line );
            std::string        item;
            int64_t            value;

            if( iss >> item >> value ) {
                if( item == "MemTotal:" ) {
                    result += value;
                    ++counter;
                } else if( item == "MemFree:" || item == "Cached:" || item == "Buffers:" ) {
                    result -= value;
                    ++counter;
                }
            } else {
                std::cerr << "/proc/meminfo has wrong format: " << line << std::endl;
                return "NaN";
            }
        }
        // if all 4 items found, result is returned else 0
        return ( ( counter == 4U ) ? std::to_string( result ) : "0" ) + " KB";
    } else {
        return "NaN";
    }
}
