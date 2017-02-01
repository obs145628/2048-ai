cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -pedantic -std=c++14")
#set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -pedantic -std=c++14 -O3 -DNDEBUG")

include_directories(../ai-cpp/include)
link_directories(${CMAKE_SOURCE_DIR}/../ai-cpp/build)

set(SRC
  src/main.cc
)


add_executable(2048-ai ${SRC})

target_link_libraries(2048-ai ai-cpp)
