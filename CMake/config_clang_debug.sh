cd ../build_clang
CC='clang -std=gnu11' CXX='clang++ -stdlib=libc++ -std=c++11' cmake .. -DCMAKE_BUILD_TYPE=Release

