cmake_minimum_required(VERSION 3.10)

project(openKBC LANGUAGES C)

set(SOURCE   
${CMAKE_SOURCE_DIR}/src
)


add_subdirectory(src)
add_subdirectory(test)

include_directories(apps)
add_executable(${CMAKE_PROJECT_NAME} apps/openKBC.c)
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -DBLOCK_SIZE=16)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra) # -g) 디버깅 옵션 주석처리
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC KBC)