cmake_minimum_required(VERSION 3.8)

project(app)

set(CMAKE_CXX_STANDARD 20)

find_package (Threads REQUIRED)

find_package(libsauros REQUIRED)
include_directories(${LIBAUROS_INCLUDE_DIRS})

add_executable(app
               main.cpp)
               
target_link_libraries(app
   PUBLIC
   dl
   ${LIBSAUROS_LIBRARIES}
   Threads::Threads)