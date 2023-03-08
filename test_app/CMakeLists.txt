
include_directories(
  ../
)

find_package (Threads REQUIRED)

add_executable(app
        ${SAUROS_SOURCES}
        main.cpp)

target_link_libraries(app
  dl
  Threads::Threads)
