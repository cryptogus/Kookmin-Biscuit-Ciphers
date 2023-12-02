cmake_minimum_required(VERSION 3.10)

#[[
if(WIN32)
    # 윈도우 플랫폼일 때 실행할 명령어
      set(CMAKE_C_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.38.33130/bin/Hostx64/x64/cl.exe")
    message("This is Windows platform.")
    # 추가적인 명령어를 여기에 추가할 수 있습니다.
endif()
]]

project(openKBC LANGUAGES C)

set(SOURCE   
${CMAKE_SOURCE_DIR}/src
)


add_subdirectory(src)
add_subdirectory(test)

include_directories(apps)
add_executable(${CMAKE_PROJECT_NAME} apps/openKBC.c)
#target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -DBLOCK_SIZE=16)
#Linux gcc 옵션
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra) # -g) 디버깅 옵션 주석처리
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC KBC)
