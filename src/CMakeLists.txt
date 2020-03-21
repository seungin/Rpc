cmake_minimum_required(VERSION 3.15)

project(Rpc VERSION 0.1.0 LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

function(conan_add_executable)
  set(options VERBOSE)
  set(oneValueArgs NAME)
  set(multiValueArgs SRCS)
  cmake_parse_arguments(TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(TARGET_VERBOSE)
    message(STATUS "TARGET_NAME: ${TARGET_NAME}")
  endif()

  add_executable(${TARGET_NAME})
  target_sources(${TARGET_NAME} PRIVATE ${TARGET_SRCS})
  target_link_libraries(${TARGET_NAME} PRIVATE ${CONAN_LIBS})
endfunction()

conan_add_executable(VERBOSE NAME StreamSocket SRCS StreamSocket.cpp)
