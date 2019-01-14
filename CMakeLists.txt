cmake_minimum_required(VERSION 3.0.0)

project(OpenCLTest)

find_package(OpenCL REQUIRED)
message(STATUS "${OpenCL_INCLUDE_DIRS}")

#include_directories(Classes ThirdParty)

include_directories(${OpenCL_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} Classes/main.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries (${PROJECT_NAME}  ${OpenCL_LIBRARY})

