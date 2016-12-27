cmake_minimum_required(VERSION 2.8)
project(code_paper)

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_CXX_FLAGS_DEBUG  "-g")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(RNG SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/random_number_generator)

add_library(Optimizable SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/optimizable)

add_library(Rectangulation SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/rectangulation)
target_link_libraries(Rectangulation Optimizable)

add_library(CircleRectangulation SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/circle_rectangulation)
target_link_libraries(CircleRectangulation ${OpenCV_LIBS} Rectangulation RNG)

add_executable(Main ${CMAKE_CURRENT_SOURCE_DIR}/src/main)
set_property(TARGET Main PROPERTY CXX_STANDARD 11)
set_property(TARGET Main PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(Main ${OpenCV_LIBS} CircleRectangulation)
