find_package(Git REQUIRED)
execute_process(COMMAND ${GIT_EXECUTABLE} log --quiet --pretty=format:'%H' -n 1
        OUTPUT_VARIABLE GIT_REV
        ERROR_QUIET)

# Check whether we got any revision (which isn't
# always the case, e.g. when someone downloaded a zip
# file from Github instead of a checkout)
if ("${GIT_REV}" STREQUAL "")
    set(GIT_REV "NA")
    set(GIT_DIFF "")
    set(GIT_TAG "NA")
    set(GIT_TXT "NA")
    set(GIT_BRANCH "NA")
else ()
    execute_process(
            COMMAND bash -c "${GIT_EXECUTABLE} diff --quiet --exit-code || echo +"
            OUTPUT_VARIABLE GIT_DIFF)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --abbrev=0
            OUTPUT_VARIABLE GIT_TAG ERROR_QUIET)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
            OUTPUT_VARIABLE GIT_BRANCH)

    string(STRIP "${GIT_REV}" GIT_REV)
    string(SUBSTRING "${GIT_REV}" 1 8 GIT_REV)
    string(STRIP "${GIT_DIFF}" GIT_DIFF)
    string(STRIP "${GIT_TAG}" GIT_TAG)
    string(STRIP "${GIT_BRANCH}" GIT_BRANCH)
endif ()

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/.git/index")

#-----------------------------------------------------------
# Generate git_rev.h
#-----------------------------------------------------------
message("   Current GIT revision is ${GIT_REV}")
file(WRITE ${PROJECT_BINARY_DIR}/gitversion.h.txt
        "//!@file ${PROJECT_BINARY_DIR}/git_rev.h - automatically generated by CMake\n"
        "//!@ingroup build_info\n"
        "//!@{\n"
        "#ifndef GIT_REV_H\n"
        "#define GIT_REV_H\n"
        "#define GIT_TAG \"${GIT_TAG}\"\n"
        "#define GIT_BRANCH \"${GIT_BRANCH}\"\n"
        "#define GIT_REV \"${GIT_REV}${GIT_DIFF}\"\n"
        "#define GIT_STR \"${GIT_TAG}-${GIT_BRANCH}-${GIT_REV}${GIT_DIFF}\"\n"
        "#endif\n"
        "//!@}\n")

set(GIT_TXT "${GIT_TAG}-${GIT_REV}")

execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_BINARY_DIR}/gitversion.h.txt ${PROJECT_BINARY_DIR}/git_rev.h)

set_source_files_properties(${PROJECT_BINARY_DIR}/git_rev.h PROPERTIES GENERATED TRUE)
