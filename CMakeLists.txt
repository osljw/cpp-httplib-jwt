cmake_minimum_required(VERSION 2.8.12) # first version with add_compile_options()
project(cpp_server)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -O3 -Wall")

add_subdirectory(SQLiteCpp)
add_subdirectory(abseil-cpp)

#include_directories("${PROJECT_SOURCE_DIR}/include")
#include_directories("$")
aux_source_directory(. CPP_SERVER_FILES)
message(${CPP_SERVER_FILES})
set(OPENSSL_DIR "/home/appops/lijianwei1/miniconda3/pkgs/openssl-1.1.1c-h516909a_0")
set(ZLIB_DIR "/home/appops/lijianwei1/miniconda3")

add_executable(cpp_server ${CPP_SERVER_FILES})
target_link_libraries(cpp_server SQLiteCpp sqlite3 pthread dl ssl crypto z absl::strings)
target_include_directories(cpp_server PRIVATE "${PROJECT_SOURCE_DIR}/include"
    "${OPENSSL_DIR}/include"
    "${ZLIB_DIR}/include"
    "abseil-cpp")
target_link_directories(cpp_server PRIVATE "${OPENSSL_DIR}/lib"
    "${ZLIB_DIR}/lib"
    "abseil-cpp")

