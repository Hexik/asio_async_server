set(src AsyncServer.cpp
        utilities.cpp)

add_library(server STATIC ${src})

add_executable(serverEcho serverEcho.cpp)
target_link_libraries(serverEcho PRIVATE server)
