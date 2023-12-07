cmake_minimum_required(VERSION 3.5)

project(tinygl)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

link_libraries(${OPENGL_LIBRARIES} GLEW::GLEW glfw fmt::fmt spdlog::spdlog)
if (TARGET glm::glm)
    link_libraries(glm::glm)
else()
    link_libraries(glm)
endif()

add_subdirectory(Catch2)

include_directories(.)
include_directories(include)
include_directories(imgui)
include_directories(Catch2/src)
include_directories(${PROJECT_BINARY_DIR}/Catch2/generated-includes)

if (MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

file(GLOB SOURCES src/*.cpp imgui/*.cpp)
list(APPEND SOURCES imgui/backends/imgui_impl_glfw.cpp)
list(APPEND SOURCES imgui/backends/imgui_impl_opengl3.cpp)
add_library(tinygl ${SOURCES})

add_executable(tinygl_mat_tests test/mat_tests.cpp)
target_link_libraries(tinygl_mat_tests PRIVATE tinygl Catch2::Catch2)

add_executable(tinygl_vec_tests test/vec_tests.cpp)
target_link_libraries(tinygl_vec_tests PRIVATE tinygl Catch2::Catch2)

file(COPY fonts DESTINATION ${CMAKE_BINARY_DIR})
