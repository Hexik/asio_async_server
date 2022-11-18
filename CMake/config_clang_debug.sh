cd ../build_clang
CC='clang-10 -std=gnu11' CXX='clang++-10 -stdlib=libc++ -std=c++11' cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_TSAN=0

