cmake_minimum_required(VERSION 3.15)

project(Rpc VERSION 0.1.0 LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

function(conan_add_executable)
  set(options VERBOSE)
  set(oneValueArgs NAME FOLDER)
  set(multiValueArgs SRCS)
  cmake_parse_arguments(TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(TARGET_VERBOSE)
    message(STATUS "TARGET_NAME: ${TARGET_NAME}")
  endif()

  source_group(TREE ${CMAKE_CURRENT_LIST_DIR} PREFIX "" FILES ${TARGET_SRCS})

  add_executable(${TARGET_NAME})
  target_sources(${TARGET_NAME} PRIVATE ${TARGET_SRCS})
  target_link_libraries(${TARGET_NAME} PRIVATE ${CONAN_LIBS})
  set_target_properties(${TARGET_NAME} PROPERTIES FOLDER CMakeTargets/${TARGET_FOLDER})
endfunction()

conan_add_executable(VERBOSE FOLDER "Basics" NAME StreamSocket SRCS StreamSocket.cpp)
conan_add_executable(VERBOSE FOLDER "Basics" NAME ServerSocket SRCS ServerSocket.cpp)
conan_add_executable(VERBOSE FOLDER "Basics" NAME DatagramSocket SRCS DatagramSocket.cpp)

conan_add_executable(VERBOSE FOLDER "Echo" NAME NonBlockEchoClient SRCS NonBlockEchoClient.cpp)
conan_add_executable(VERBOSE FOLDER "Echo" NAME NonBlockEchoServer SRCS NonBlockEchoServer.cpp)
conan_add_executable(VERBOSE FOLDER "Echo" NAME TcpEchoClient SRCS TcpEchoClient.cpp)
conan_add_executable(VERBOSE FOLDER "Echo" NAME TcpEchoServer SRCS TcpEchoServer.cpp)
conan_add_executable(VERBOSE FOLDER "Echo" NAME UdpEchoClient SRCS UdpEchoClient.cpp)
conan_add_executable(VERBOSE FOLDER "Echo" NAME UdpEchoServer SRCS UdpEchoServer.cpp)

conan_add_executable(VERBOSE FOLDER "Framework" NAME ReactorEchoClient SRCS ReactorEchoClient.cpp)
conan_add_executable(VERBOSE FOLDER "Framework" NAME ReactorEchoServer SRCS ReactorEchoServer.cpp)
conan_add_executable(VERBOSE FOLDER "Framework" NAME TcpServer SRCS TcpServer.cpp)
