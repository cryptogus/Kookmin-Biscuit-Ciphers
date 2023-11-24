cmake_minimum_required(VERSION 3.10)

project(AES_ECB_CBC LANGUAGES C)

set(SOURCE
    
${CMAKE_SOURCE_DIR}/src
${CMAKE_SOURCE_DIR}/mode
)


add_subdirectory(src)
add_subdirectory(test)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE} main.c)