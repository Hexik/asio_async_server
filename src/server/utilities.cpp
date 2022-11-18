//
// Created by mf on 18.11.22.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "utilities.h"

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

std::string getResponse( const std::string& cmd )
{
    if( cmd == "cpu" ) {
        return getCPU();
    }
    if( cmd == "mem" ) {
        return getMEM();
    }
    return "NA";
}

std::string getCPU() { return std::string( "13" ); }

std::string getMEM()
{
    std::ifstream ifs( "/proc/meminfo" );

    if( ifs ) {
        std::string line;
        auto        counter = 0U;
        int64_t     result  = 0;

        while( std::getline( ifs, line ) && counter < 4U ) {
            std::istringstream iss( line );
            std::string        name;
            int64_t            value;

            if( iss >> name >> value ) {
                if( name == "MemTotal:" ) {
                    result += value;
                    ++counter;
                } else if( name == "MemFree:" ) {
                    result -= value;
                    ++counter;
                } else if( name == "Cached:" ) {
                    result -= value;
                    ++counter;
                } else if( name == "Buffers:" ) {
                    result -= value;
                    ++counter;
                }
            } else {
                std::cerr << "/proc/meminfo" << "has wrong format: " << line << std::endl;
                return "NaN";
            }
        }
        return ( counter == 4U ) ? std::to_string( result ) : "0";
    } else {
        return "NaN";
    }
}
