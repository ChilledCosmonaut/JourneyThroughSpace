cmake_minimum_required(VERSION 3.21)
project(code)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(externalCode/glad)
add_subdirectory(externalCode/glfw)
add_subdirectory(externalCode/glm)
add_subdirectory(externalCode/assimp)
add_subdirectory(game)


