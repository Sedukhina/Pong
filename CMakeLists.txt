cmake_minimum_required(VERSION 3.25)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/libs/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(PONG VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_definitions("BIN_ROOT=${CMAKE_BINARY_DIR}")

set(PONG_SRC "Pong.cpp")
add_executable(Pong ${PONG_SRC})

set(ASSETS_DIR ${CMAKE_BINARY_DIR}/Assets)
target_include_directories(Pong PUBLIC ${ASSETS_DIR})
file(GLOB assets "Assets/*")

foreach(file ${assets})
    message(STATUS "Copying resource ${file}")
    file(COPY ${file} DESTINATION ${ASSETS_DIR})
endforeach()


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(Engine)
target_link_libraries(Pong PUBLIC Engine)