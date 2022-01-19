cmake_minimum_required(VERSION 3.18)

set(SOURCE_FILES src/main.cpp src/shader.cpp src/shader.h FileManager/FileManager.cpp FileManager/FileManager.h)

set(EXE_FILE Game)

add_executable(${EXE_FILE} ${SOURCE_FILES})

target_compile_features(${EXE_FILE} PRIVATE cxx_std_17)

target_link_libraries(${EXE_FILE} PRIVATE glad glfw glm assimp)

