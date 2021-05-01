cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)     # Use c++11
set(CMAKE_VERBOSE_MAKEFILE ON) # Debugging the build system

project(DoodleJump)
set(SOURCES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# Force static linking with sfml
set(BUILD_SHARED_LIBS OFF)
set(SFML_USE_STATIC_STD_LIBS ON)

set(SFML_BUILD_DOC OFF)      # Don't build docs
set(SFML_BUILD_EXAMPLES OFF) # Don't build examples
set(SFML_BUILD_NETWORK OFF)  # Don't build sfml networking

add_subdirectory(lib/sfml)
target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} stdc++-6)
endif()