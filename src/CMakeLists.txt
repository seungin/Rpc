cmake_minimum_required(VERSION 3.15)

project(Rpc VERSION 0.1.0 LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(app main.cpp)
target_link_libraries(app PRIVATE ${CONAN_LIBS})
