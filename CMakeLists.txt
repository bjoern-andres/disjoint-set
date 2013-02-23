cmake_minimum_required(VERSION 2.6)
project(partition)
include_directories(include)
file(GLOB headers include/andres/*.hxx)
enable_testing()

##############################################################################
# Doxygen
##############################################################################
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    message(STATUS "Doxygen found")
else()
    message("doxygen not found")
endif()

##############################################################################
# targets: partition
##############################################################################
add_executable(test-partition src/unittest/partition.cxx ${headers})
add_test(test-partition test-partition)

if(DOXYGEN_FOUND)
    configure_file("${partition_SOURCE_DIR}/doxygen/doxyfile-partition.in" "${partition_BINARY_DIR}/doxyfile-partition" @ONLY IMMEDIATE)
    add_custom_target(doc-partition ALL COMMAND ${DOXYGEN} "${partition_BINARY_DIR}/doxyfile-partition")
endif()
