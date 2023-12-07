cmake_minimum_required(VERSION 3.10)
project(dwalls)

set(CMAKE_CXX_STANDARD 14)

find_package(Qt5Widgets REQUIRED)

include_directories(/usr/include/libheif)
link_directories(/usr/lib/x86_64-linux-gnu)

include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})

add_executable(dwalls src/main.cpp src/heic_processor/heic_processor.cpp)

target_link_libraries(dwalls Qt5::Widgets heif jpeg)