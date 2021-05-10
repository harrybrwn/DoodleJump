cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)     # Use c++11

project(DoodleJump)
file(GLOB SOURCES CONFIGURE_DEPENDS *.cpp *.hpp *.h)
add_executable(${PROJECT_NAME} ${SOURCES})

# Force static linking with sfml
set(BUILD_SHARED_LIBS OFF)
set(SFML_USE_STATIC_STD_LIBS ON)

set(SFML_BUILD_DOC OFF)      # Don't build docs
set(SFML_BUILD_EXAMPLES OFF) # Don't build examples

add_subdirectory(lib/sfml)
target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} stdc++-6)
endif()