cmake_minimum_required(VERSION 3.0.0)

project(OpenCLTest)

include_directories(Classes)

add_executable(${PROJECT_NAME} Classes/main.cpp)
