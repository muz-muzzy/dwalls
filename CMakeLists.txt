cmake_minimum_required(VERSION 3.10)
project(your_project_name)

set(CMAKE_CXX_STANDARD 14)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

# Find the heif library
find_package(heif REQUIRED)

include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})

# Add your source files
add_executable(your_project_name src/main.cpp src/heic_processor/heic_processor.cpp)

# Link the libraries to your executable
target_link_libraries(your_project_name Qt5::Widgets ${HEIF_LIBRARY})