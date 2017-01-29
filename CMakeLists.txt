cmake_minimum_required(VERSION 3.0)
project(2048AI)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -pedantic -std=c++14 -O3 -DNDEBUG")

include(ExternalProject)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/ext)


ExternalProject_Add(git-ai-cpp
    GIT_REPOSITORY https://github.com/obs145628/ai-cpp.git
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

set(SRC
  src/main.cc
)

add_executable(2048-ai ${SRC})
add_dependencies(2048-ai git-ai-cpp)
target_link_libraries(2048-ai ai-cpp)
