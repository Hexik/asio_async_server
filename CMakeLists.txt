cmake_minimum_required(VERSION 3.0.0)

message("")
message(STATUS "remote_top CMake configuration")
message("")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(CheckCXXCompilerFlag)

############################################################
# Main project
############################################################
project(remote_top)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_FLAGS_TMP "")
set(CMAKE_CXX_FLAGS_TMP "")
set(TEST_SOURCES "")
set(HEADER_DIR "")

if (CMAKE_HOST_UNIX)
    message(STATUS "Unix host")
elseif (CMAKE_HOST_WIN32)
    message(STATUS "Windows host")
else ()
    message(FATAL_ERROR "Unsupported OS/Platform")
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# abort if the build type is not specified by -DCMAKE_BUILD_TYPE=
if (NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "No Cmake build type specified. Options are Debug, Release")
endif ()
if (NOT CMAKE_BUILD_TYPE MATCHES "Debug" AND NOT CMAKE_BUILD_TYPE MATCHES "Release")
    message(FATAL_ERROR "Invalid Cmake build type specified. Options are Debug, Release")
endif ()

############################################################
# Find packages
############################################################
# Find packages - generic
#-----------------------------------------------------------
if (CMAKE_HOST_UNIX AND NOT WITH_DOC)
    find_program(CCACHE_VAR ccache)
    if (CCACHE_VAR)
        SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
        message(STATUS "Using ccache")
    endif ()
endif ()

#-----------------------------------------------------------
# Find packages - Git
#-----------------------------------------------------------
include(GetGitVersion)
message("   Version  : ${GIT_TXT}")

#-----------------------------------------------------------
# Find packages - Boost
#-----------------------------------------------------------
find_package(Boost REQUIRED COMPONENTS system )
message("   Boost    : ${Boost_VERSION_STRING}")

############################################################
# Include directories
############################################################
SET(CMAKE_INCLUDE_CURRENT_DIR OFF)

############################################################
# Preprocessor definitions
############################################################
add_definitions(
        -D_GLIBCXX_USE_CXX11_ABI=0
)

if (WITH_DOC)
    add_definitions(
            -DCHECK_DOCS
    )
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(
            -DDEBUG
    )
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Release")
    add_definitions(
            -DNDEBUG
    )
endif ()

############################################################
# Compiler options
############################################################

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

macro(add_c_compiler_flag FLAG)
    string(REGEX REPLACE "-" "" SFLAG ${FLAG})
    check_c_compiler_flag(${FLAG} COMPILER_SUPPORT_${SFLAG})
    if (COMPILER_SUPPORT_${SFLAG})
        set(CMAKE_C_FLAGS_TMP "${CMAKE_C_FLAGS_TMP} ${FLAG}")
    endif ()
endmacro(add_c_compiler_flag)

macro(add_cxx_compiler_flag FLAG)
    string(REGEX REPLACE "-" "" SFLAG ${FLAG})
    check_cxx_compiler_flag(${FLAG} COMPILER_SUPPORT_${SFLAG})
    if (COMPILER_SUPPORT_${SFLAG})
        set(CMAKE_CXX_FLAGS_TMP "${CMAKE_CXX_FLAGS_TMP} ${FLAG}")
    endif ()
endmacro(add_cxx_compiler_flag)

macro(add_compiler_flag FLAG)
    add_cxx_compiler_flag(${FLAG})
    add_c_compiler_flag(${FLAG})
endmacro(add_compiler_flag)

check_cxx_compiler_flag("-Werror" COMPILER_SUPPORT_WERROR)

add_compiler_flag("-fdata-sections")
add_compiler_flag("-ffunction-sections")
add_compiler_flag("-minline-all-stringops")
add_compiler_flag("-fno-check-pointer-bounds")
add_compiler_flag("-fno-common")
add_compiler_flag("-fexceptions")
add_compiler_flag("-fno-lto")
add_compiler_flag("-funsigned-char")
add_compiler_flag("-m64")
add_compiler_flag("-pedantic")
add_cxx_compiler_flag("-fno-rtti")

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    add_compiler_flag("-Weverything")
    add_compiler_flag("-fcomment-block-commands=cond")
    add_compiler_flag("-Wno-c++98-compat")
    add_compiler_flag("-Wno-c++98-compat-pedantic")
    add_compiler_flag("-Wno-padded")

else ()

    add_compiler_flag("-Wabi=11")
    add_compiler_flag("-Wabsolute-value")
    add_compiler_flag("-Waddress")
    add_compiler_flag("-Waddress-of-packed-member")
    add_compiler_flag("-Wno-aggregate-return")
    add_compiler_flag("-Wall")
    add_compiler_flag("-Warray-bounds")
    add_compiler_flag("-Wattribute-alias")
    add_compiler_flag("-Wattributes")
    add_compiler_flag("-Wbool-compare")
    add_compiler_flag("-Wbuiltin-macro-redefined")
    add_compiler_flag("-Wcast-align")
    add_compiler_flag("-Wcast-qual")
    add_compiler_flag("-Wcomment")
    add_compiler_flag("-Wcomments")
    add_compiler_flag("-Wconversion")
    add_compiler_flag("-Wdangling-else")
    add_compiler_flag("-Wdeprecated")
    add_compiler_flag("-Wdeprecated-declarations")
    add_compiler_flag("-Wdisabled-optimization")
    add_compiler_flag("-Wdiv-by-zero")
    add_compiler_flag("-Wdouble-promotion")
    add_compiler_flag("-Wduplicated-branches")
    add_compiler_flag("-Wduplicated-cond")
    add_compiler_flag("-Wempty-body")
    add_compiler_flag("-Wendif-labels")
    add_compiler_flag("-Wenum-compare")
    add_compiler_flag("-Wexit-time-destructors")
    add_compiler_flag("-Wextra")
    add_compiler_flag("-Wfloat-conversion")
    add_compiler_flag("-Wformat")
    add_compiler_flag("-Wformat=2")
    add_compiler_flag("-Wformat-extra-args")
    add_compiler_flag("-Wformat-overflow=2")
    add_compiler_flag("-Wformat-nonliteral")
    add_compiler_flag("-Wformat-pedantic")
    add_compiler_flag("-Wformat-security")
    add_compiler_flag("-Wformat-truncation=2")
    add_compiler_flag("-Wformat-y2k")
    add_compiler_flag("-Wformat-zero-length")
    add_compiler_flag("-Wfour-char-constants")
    add_compiler_flag("-Wframe-larger-than=10000000")
    add_compiler_flag("-Wglobal-constructors")
    add_compiler_flag("-Wchar-subscripts")
    add_compiler_flag("-Wif-not-aligned")
    add_compiler_flag("-Wignored-qualifiers")
    add_compiler_flag("-Wimplicit-fallthrough")
    add_compiler_flag("-Wimport")
    add_compiler_flag("-Winit-self")
    add_compiler_flag("-Wno-inline")
    add_compiler_flag("-Wint-to-pointer-cast")
    add_compiler_flag("-Winvalid-pch")
    add_compiler_flag("-Wlogical-not-parentheses")
    add_compiler_flag("-Wlogical-op")
    add_compiler_flag("-Wno-long-long")
    add_compiler_flag("-Wmain")
    add_compiler_flag("-Wmissing-attributes")
    add_compiler_flag("-Wmissing-braces")
    add_compiler_flag("-Wmissing-declarations")
    add_compiler_flag("-Wmissing-field-initializers")
    add_compiler_flag("-Wmissing-format-attribute")
    add_compiler_flag("-Wmissing-include-dirs")
    add_compiler_flag("-Wmissing-noreturn")
    add_compiler_flag("-Wmultichar")
    add_compiler_flag("-Wmultistatement-macros")
    add_compiler_flag("-Wnarrowing")
    add_compiler_flag("-Wnonnull")
    add_compiler_flag("-Wnull-dereference")
    add_compiler_flag("-Wodr")
    add_compiler_flag("-Wno-error=overflow")
    add_compiler_flag("-Woverlength-strings")
    add_compiler_flag("-Wpacked")
    add_compiler_flag("-Wpacked-not-aligned")
    add_compiler_flag("-Wno-padded")
    add_compiler_flag("-Wparentheses")
    add_compiler_flag("-Wpedantic")
    add_compiler_flag("-Wpointer-arith")
    add_compiler_flag("-Wpointer-compare")
    add_compiler_flag("-Wpragmas")
    add_compiler_flag("-Wredundant-decls")
    add_compiler_flag("-Wrestrict")
    add_compiler_flag("-Wreturn-type")
    add_compiler_flag("-Wsequence-point")
    add_compiler_flag("-Wshadow")
    add_compiler_flag("-Wshift-count-negative")
    add_compiler_flag("-Wshift-count-overflow")
    add_compiler_flag("-Wshift-negative-value")
    add_compiler_flag("-Wshift-overflow")
    add_compiler_flag("-Wsign-compare")
    add_compiler_flag("-Wsign-conversion")
    add_compiler_flag("-Wsizeof-array-argument")
    add_compiler_flag("-Wsizeof-pointer-memaccess")
    add_compiler_flag("-Wstack-protector")
    add_compiler_flag("-Wstrict-aliasing")
    add_compiler_flag("-Wstrict-aliasing=0")
    add_compiler_flag("-Wstrict-aliasing=1")
    add_compiler_flag("-Wstrict-aliasing=2")
    add_compiler_flag("-Wstrict-overflow")
    add_compiler_flag("-Wstrict-overflow=0")
    add_compiler_flag("-Wstrict-overflow=1")
    add_compiler_flag("-Wstrict-overflow=2")
    add_compiler_flag("-Wstrict-overflow=3")
    add_compiler_flag("-Wstrict-overflow=4")
    add_compiler_flag("-Wstrict-overflow=5")
    add_compiler_flag("-Wstringop-truncation")
    add_compiler_flag("-Wswitch")
    add_compiler_flag("-Wswitch-bool")
    add_compiler_flag("-Wno-switch-default")
    add_compiler_flag("-Wswitch-enum")
    add_compiler_flag("-Wtrigraphs")
    add_compiler_flag("-Wtype-limits")
    add_compiler_flag("-Wundef")
    add_compiler_flag("-Wuninitialized")
    add_compiler_flag("-Wno-unknown-pragmas")
    add_compiler_flag("-Wunreachable-code")
    add_compiler_flag("-Wunused")
    add_compiler_flag("-Wunused-but-set-parameter")
    add_compiler_flag("-Wunused-but-set-variable")
    add_compiler_flag("-Wunused-function")
    add_compiler_flag("-Wunused-label")
    add_compiler_flag("-Wunused-local-typedefs")
    add_compiler_flag("-Wunused-parameter")
    add_compiler_flag("-Wunused-result")
    add_compiler_flag("-Wunused-value")
    add_compiler_flag("-Wunused-variable")
    add_compiler_flag("-Wvarargs")
    add_compiler_flag("-Wvariadic-macros")
    add_compiler_flag("-Wvla")
    add_compiler_flag("-Wvolatile-register-var")
    add_compiler_flag("-Wwrite-strings")

    add_cxx_compiler_flag("-faligned-new")
    add_cxx_compiler_flag("-Wabi-tag")
    add_cxx_compiler_flag("-Wbad-function-cast")
    add_cxx_compiler_flag("-Wc++0x-compat")
    add_cxx_compiler_flag("-Wc++11-compat")
    add_cxx_compiler_flag("-Wc++14-compat")
    add_cxx_compiler_flag("-Wno-cast-align=strict")
    add_cxx_compiler_flag("-Wcast-qual")
    add_cxx_compiler_flag("-Wclass-conversion")
    add_cxx_compiler_flag("-Wclass-memaccess")
    add_cxx_compiler_flag("-Wconversion-null")
    add_cxx_compiler_flag("-Wctor-dtor-privacy")
    add_cxx_compiler_flag("-Wdelete-incomplete")
    add_cxx_compiler_flag("-Wdelete-non-virtual-dtor")
    add_cxx_compiler_flag("-Wdeprecated")
    add_cxx_compiler_flag("-Wdeprecated-copy")
    add_cxx_compiler_flag("-Wno-effc++")
    add_cxx_compiler_flag("-Wnoexcept-type")
    add_cxx_compiler_flag("-Winit-list-lifetime")
    add_cxx_compiler_flag("-Winvalid-offsetof")
    add_cxx_compiler_flag("-Wnon-virtual-dtor")
    add_cxx_compiler_flag("-Wno-suggest-attribute=format")
    add_cxx_compiler_flag("-Wno-suggest-attribute=noreturn")
    add_cxx_compiler_flag("-Wold-style-cast")
    add_cxx_compiler_flag("-Woverloaded-virtual")
    add_cxx_compiler_flag("-Wpessimizing-move")
    add_cxx_compiler_flag("-Wredundant-move")
    add_cxx_compiler_flag("-Wreorder")
    add_cxx_compiler_flag("-Wsign-promo")
    add_cxx_compiler_flag("-Wsuggest-attribute=const")
    add_cxx_compiler_flag("-Wsuggest-attribute=pure")
    add_cxx_compiler_flag("-Wsuggest-final-methods")
    add_cxx_compiler_flag("-Wsuggest-final-types")
    add_cxx_compiler_flag("-Wsynth")
    add_cxx_compiler_flag("-Wunused-macros")
    add_cxx_compiler_flag("-Wuseless-cast")
endif ()

if (CMAKE_HOST_UNIX)
    add_compiler_flag("-pthread")
endif ()

if (ENABLE_ASAN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=gold")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

    if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=implicit-conversion")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=integer")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=leak")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=nullability")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=unsigned-integer-overflow")
    endif ()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

    if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=implicit-conversion")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=integer")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=nullability")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=unsigned-integer-overflow")
    endif ()
endif ()

if (ENABLE_MSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -fsanitize-memory-track-origins=2 -stdlib=libc++ -L/home/mf/src/libcxx_msan/lib -lc++abi -Wl,-rpath,/home/mf/src/libcxx_msan/lib ")
endif ()

if (ENABLE_TSAN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=gold")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
endif ()

if (ENABLE_USAN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=gold")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    #    add_compiler_flag( "-ftrapv" )
    add_compiler_flag("-fno-omit-frame-pointer ")
    add_compiler_flag("-fstack-protector")
    add_compiler_flag("-fstack-protector-all")
    add_compiler_flag("-Wstack-protector")
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Release")
    add_compiler_flag("-fno-strict-overflow")
    add_compiler_flag("-fomit-frame-pointer ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold ")
    add_compiler_flag("-g")
endif ()

# The -ansi flag must be added last, otherwise it is also used as a linker flag by check_cxx_compiler_flag making it fails
# Moreover we should not set both -strict-ansi and -ansi
check_cxx_compiler_flag("-strict-ansi" COMPILER_SUPPORT_STRICTANSI)
add_compiler_flag("-Qunused-arguments")        # disable clang warning: argument unused during compilation: '-ansi'

if (COMPILER_SUPPORT_STRICTANSI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -strict-ansi")
else ()
    add_cxx_compiler_flag("-ansi")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_TMP} ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_TMP} ${CMAKE_C_FLAGS}")

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ferror-limit=0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}   -ferror-limit=0")
    add_compiler_flag("-Waggregate-return")
endif ()

# adding -Werror turns such warnings into errors
if (COMPILER_SUPPORT_WERROR)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
endif ()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections -Wl,-Map=output.map")

############################################################
# Project components
############################################################
include_directories(.)
include_directories(./src)
include_directories(${PROJECT_BINARY_DIR})

add_subdirectory(src)

set(SRCS
        src/main.cpp
        )

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SRCS})

############################################################
# Link targets
############################################################
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        server
        ${Boost_LIBRARIES}
        )

############################################################
# Post build step
############################################################

############################################################
# Summary
############################################################
message("")
message("   Config summary")
message("   ======================")
message("   Project   : ${CMAKE_PROJECT_NAME}")
message("   Build     : ${CMAKE_BUILD_TYPE}")
message("   Boost lib : ${Boost_LIBRARIES}")
message("")
