cmake_minimum_required(VERSION 3.20)
project(RenderLite)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

file(GLOB_RECURSE RENDERLITE_HEADERS "include/*.hpp")
file(GLOB_RECURSE RENDERLITE_SOURCES "source/*.cpp")

add_executable(RenderLite
    ${RENDERLITE_HEADERS}
    ${RENDERLITE_SOURCES}
)

target_include_directories(RenderLite PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(RenderLite PUBLIC
    ${SDL2_LIBRARIES}
)
