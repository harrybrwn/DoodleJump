cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11) # Use c++11

project(DoodleJump)
set(SOURCES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(lib/sfml)
target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
    # sfml-audio
)
