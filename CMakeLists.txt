cmake_minimum_required(VERSION 3.0)
project(2048AI)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -pedantic -std=c++14 -O3 -DNDEBUG")



add_subdirectory(ext/ai-cpp/src)

include_directories(ext/ai-cpp/include)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
include_directories(${SFML_INCLUDE_DIR})


set(SRC
  src/grid.cc
  src/heur.cc
  src/main.cc
  src/monte-carlo.cc
  src/world.cc
)

add_executable(2048-ai ${SRC})
add_dependencies(2048-ai ai-cpp)
target_link_libraries(2048-ai ai-cpp ${SFML_LIBRARIES})

add_subdirectory(tests)
add_subdirectory(pyx)
