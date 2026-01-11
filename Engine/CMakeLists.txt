file(GLOB_RECURSE ENGINE_SRC "*.h" "*.cpp")
add_library(Engine STATIC ${ENGINE_SRC})

set(ASSETS_DIR ${CMAKE_BINARY_DIR}/Assets/)
add_compile_definitions("ASSETS_DIR=${ASSETS_DIR}")

# Window and graphics libs
find_package(glfw3 CONFIG REQUIRED)
target_include_directories(Engine PRIVATE ${glfw3_INCLUDE_DIRS})
target_link_libraries(Engine PUBLIC glfw)
find_package(GLEW REQUIRED)
target_include_directories(Engine PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(Engine PUBLIC GLEW::GLEW)

# Graphics calculations
find_package(glm REQUIRED)
target_include_directories(Engine PRIVATE ${glm_INCLUDE_DIRS})
target_link_libraries(Engine PUBLIC glm::glm)

# Images/Textures loading lib
find_package(stb REQUIRED)
target_include_directories(Engine PRIVATE ${stb_INCLUDE_DIRS})

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
