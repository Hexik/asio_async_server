![example workflow](https://github.com/Hexik/asio_async_server/actions/workflows/cmake.yml/badge.svg)
# Simple Async TCP server

Boost asio is used as network library


## Build

### Prerequisites
Linux operating system and toolchain, tested on Ubuntu

Installed CMake 3.x.x, Git, GCC >= 5 / Clang >= 9, libboost-all-dev >= 1.71

Compiler should support at least C++11, do not use GCC 8.x, this version is buggy.

Check compiler version before you start build configuration

```bash
g++ -v
```

Although this is not a recommended practice, I have published local configuration scripts, you can find them in the CMake folder. Use them as inspiration or as the default code for your editing and use. Another information can be found in .github/workflows/cmake.yml.

#### Used CMake variables and its values

Mandatory

- CMAKE_BUILD_TYPE ... Debug, Release

Optional

- CMAKE_CXX_COMPILER ... use if env CXX is not set

### Linux build

```bash
git clone git@github.com:Hexik/asio_async_server.git
cd asio_async_server
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

The client and server binaries will be generated:

- `./remote_top` daemon server
- `./src/server/serverEcho` server running in console, data dump
- `./src/client/client` simple client


## Usage

### Start daemon

```bash
sudo ./remote_top
```

### Run single client

This is basic usage
```bash
./src/client/client 127.0.0.1 mem cpu
```

### Run multiple clients parallel

```bash
./src/client/client 127.0.0.1 mem cpu cpu mem cpu mem cpu cpu & ./src/client/client 127.0.0.1 mem cpu mem cpu cpu cpu mem mem & ./src/client/client 127.0.0.1 a b c d e f && fg
```

### Kill daemon

```bash
sudo pkill remote_top
```

### Console server

Start server in one terminal
```bash
./src/server/serverEcho
```
open the second terminal and use client app as above


## License
Project is distributed under the Boost Software licence 1.0

[![License](https://img.shields.io/badge/License-Boost_1.0-lightblue.svg)](https://www.boost.org/LICENSE_1_0.txt)

-   Copyright 2022 © Miroslav Fontan
