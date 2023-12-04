list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Dwalls)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the required packages
find_package(heif REQUIRED)
find_package(JPEG REQUIRED)

# Include the directories for the packages
include_directories(${HEIF_INCLUDE_DIRS} ${JPEG_INCLUDE_DIR})

# Add the common source files
set(SOURCES src/main.cpp)

# Conditionally add the platform-specific source files
if(WIN32)
    list(APPEND SOURCES src/wallpaper_manager/windows/windows_manager.cpp)
else()
    list(APPEND SOURCES src/wallpaper_manager/linux/linux_manager.cpp)
endif()

# Add the executable
add_executable(Dwalls ${SOURCES})

# Link the libraries to the executable
target_link_libraries(Dwalls ${HEIF_LIBRARIES} ${JPEG_LIBRARIES})