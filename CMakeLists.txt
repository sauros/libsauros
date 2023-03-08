cmake_minimum_required(VERSION 3.5)

project (libsauros)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

#
# Options
#
option(COMPILE_TESTS   "Execute unit tests" ON)
option(WITH_ASAN       "Compile with ASAN" OFF)
option(ENABLE_PROFILER "Enable code profiler" OFF)

#
# Setup build type 'Release vs Debug'
#
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

#
# Locate CPPUTest
#
if(DEFINED ENV{CPPUTEST_HOME})
    message(STATUS "Using CppUTest home: $ENV{CPPUTEST_HOME}")
    set(CPPUTEST_INCLUDE_DIRS $ENV{CPPUTEST_HOME}/include)
    set(CPPUTEST_LIBRARIES $ENV{CPPUTEST_HOME}/lib)
    set(CPPUTEST_LDFLAGS CppUTest CppUTestExt)
else()
    find_package(PkgConfig REQUIRED)
    pkg_search_module(CPPUTEST REQUIRED cpputest>=3.8)
    message(STATUS "Found CppUTest version ${CPPUTEST_VERSION}")
endif()

#
# Setup ASAN
#
if(WITH_ASAN)
  set(CMAKE_CX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
  set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

if(ENABLE_PROFILER)
   add_compile_definitions(PROFILER_ENABLED)
endif()

#
#  Update version file
#
include("cmake/CheckGit.cmake")

include(${PROJECT_SOURCE_DIR}/cmake/SetEnv.cmake)

include_directories("libsauros")

if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DNDEBUG)
endif()

set(SAUROS_SOURCES
   ${PROJECT_SOURCE_DIR}/libsauros/environment.cpp
   ${PROJECT_SOURCE_DIR}/libsauros/cell.cpp
   ${PROJECT_SOURCE_DIR}/libsauros/driver.cpp
   ${PROJECT_SOURCE_DIR}/libsauros/profiler.cpp
   ${PROJECT_SOURCE_DIR}/libsauros/capi/capi.cpp
   ${PROJECT_SOURCE_DIR}/libsauros/RLL/rll_wrapper.cpp
   ${PROJECT_SOURCE_DIR}/libsauros/system/system.cpp
   ${PROJECT_SOURCE_DIR}/libsauros/front/parser.cpp
   ${PROJECT_SOURCE_DIR}/libsauros/package/package.cpp
   ${PROJECT_SOURCE_DIR}/libsauros/processor/processor.cpp
   ${PROJECT_SOURCE_DIR}/libsauros/processor/processor_load_package.cpp
   ${PROJECT_SOURCE_DIR}/libsauros/processor/processor_builtins.cpp
)

set(PARALLEL_HASHMAP_HEADERS
   ${PROJECT_SOURCE_DIR}/libsauros/parallel_hashmap/btree.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/parallel_hashmap/meminfo.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/parallel_hashmap/phmap_base.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/parallel_hashmap/phmap_bits.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/parallel_hashmap/phmap_config.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/parallel_hashmap/phmap_dump.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/parallel_hashmap/phmap_fwd_decl.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/parallel_hashmap/phmap_utils.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/parallel_hashmap/phmap.hpp
)

set(LINENOISE_HEADERS
   ${PROJECT_SOURCE_DIR}/libsauros/linenoise/linenoise.hpp
)

set(HEADERS
   ${PROJECT_SOURCE_DIR}/libsauros/profiler.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/format.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/front/parser.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/driver.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/rang.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/capi/capi.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/RLL/rll_wrapper.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/system/system.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/package/package.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/processor/processor.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/environment.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/cell.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/types.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/version.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/sauros.hpp
   ${PROJECT_SOURCE_DIR}/libsauros/RLL/RLL.h
   ${PARALLEL_HASHMAP_HEADERS}
   ${LINENOISE_HEADERS}
)

set(SOURCES
    ${SAUROS_SOURCES}
)

#
# Configure Library
#
include(${PROJECT_SOURCE_DIR}/cmake/LibraryConfig.cmake)

#
# Configure Install
#
include(${PROJECT_SOURCE_DIR}/cmake/InstallConfig.cmake)

#
# Tests
#
if(COMPILE_TESTS)

  # Run unit tests
  add_subdirectory(tests)

  # Create a stub app so we can run script tests
  add_subdirectory(test_app)
endif()