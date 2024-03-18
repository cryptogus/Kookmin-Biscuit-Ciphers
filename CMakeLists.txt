cmake_minimum_required(VERSION 3.10)

#[[
if(WIN32)
    # 윈도우 플랫폼일 때 실행할 명령어
    # MSVC x64 컴파일러 위치
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

# 사용자 정의 옵션: USE_OPENSSL
option(USE_OPENSSL "Use OpenSSL for encryption" ON)

# OpenSSL을 사용하는 경우
if(USE_OPENSSL)
    # OpenSSL을 찾습니다.
    # REQUIRED 키워드가 사용되었기 때문에 OpenSSL을 찾을 수 없으면 CMake는 에러를 발생시키고 빌드를 중단합니다.
    find_package(OpenSSL REQUIRED)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -DUSE_OPENSSL)
    if(OPENSSL_FOUND)
        message(STATUS "OpenSSL found. Version: ${OPENSSL_VERSION}")
        include_directories(${OPENSSL_INCLUDE_DIR})

        # 여기에 프로젝트 소스 코드 및 빌드 설정을 추가합니다.

        # 예를 들어, 다음과 같이 라이브러리를 링크할 수 있습니다.
        target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${OPENSSL_LIBRARIES})
    else()
        message(FATAL_ERROR "OpenSSL not found. Please install OpenSSL.")
    endif()
endif()