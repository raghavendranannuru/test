cmake_minimum_required(VERSION 3.10)
project(WebsiteConnection)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)

# Find libcurl
find_package(CURL REQUIRED)
if(CURL_FOUND)
    message(STATUS "Found CURL version: ${CURL_VERSION_STRING}")
    message(STATUS "CURL libraries: ${CURL_LIBRARIES}")
    message(STATUS "CURL include dirs: ${CURL_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "CURL not found. Please install libcurl-dev or libcurl-devel")
endif()

# Include directories
include_directories(${CURL_INCLUDE_DIRS})

# Create executable
add_executable(website_connection 
    main.cpp 
    http_client.cpp
)

# Link libraries
target_link_libraries(website_connection ${CURL_LIBRARIES})

# Add compiler flags for better debugging and warnings
target_compile_options(website_connection PRIVATE 
    -Wall 
    -Wextra 
    -g
)

# Install target
install(TARGETS website_connection
    RUNTIME DESTINATION bin
)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ flags: ${CMAKE_CXX_FLAGS}")