set(AES128 "")

# 목적파일 생성
add_library(AES128 OBJECT aes128.c)
target_include_directories(AES128 PRIVATE ${CMAKE_CURRENT_LIST_DIR})