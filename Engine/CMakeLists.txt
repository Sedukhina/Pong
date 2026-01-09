file(GLOB_RECURSE ENGINE_SRC "*.cpp")
add_library(Engine STATIC ${ENGINE_SRC})

# Window and graphics libs
find_package(glfw3 CONFIG REQUIRED)
target_include_directories(Engine PRIVATE ${glfw3_INCLUDE_DIRS})
target_link_libraries(Engine PUBLIC glfw)
find_package(GLEW REQUIRED)
target_include_directories(Engine PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(Engine PUBLIC GLEW::GLEW)

# Enum to String lib for logger
find_package(magic_enum CONFIG REQUIRED)
target_link_libraries(Engine PRIVATE magic_enum::magic_enum)

set(SHADERS_DIR ${CMAKE_BINARY_DIR}/Shaders)
target_include_directories(Engine PUBLIC ${SHADERS_DIR})
file(GLOB shaders "render/Shaders/*.vert" "render/Shaders/*.frag")

foreach(file ${shaders})
    message(STATUS "Copying resource ${file}")
    file(COPY ${file} DESTINATION ${SHADERS_DIR})
endforeach()

target_include_directories(Engine PUBLIC "${CURRENT_SOURCE_DIR}")
