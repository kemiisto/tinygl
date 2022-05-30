cmake_minimum_required(VERSION 3.2)

if(WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "c:/Users/$ENV{USERNAME}/Downloads/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

project(tinygl)

set(CMAKE_CXX_STANDARD 23)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

link_libraries(GLEW::GLEW glfw glm::glm fmt::fmt spdlog::spdlog)
include_directories(include)

if (MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

file(GLOB SOURCES src/*.cpp)
add_library(tinygl ${SOURCES})
