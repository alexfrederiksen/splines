cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DENABLE_DEBUG_MACRO")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(spline STATIC ${CMAKE_SOURCE_DIR}/src/Spline.cpp)

add_executable(splineViewer ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(splineViewer spline SDL2)
