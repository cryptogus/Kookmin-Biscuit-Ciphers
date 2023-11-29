set(AES128 "")

# 목적파일 생성
add_library(aes OBJECT aes.c)
target_include_directories(aes128 PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set(AES ${AES} $<TARGET_OBJECTS:aes>)

# 부모 cmake에 자식 변수 알려주기
set(AES ${AES} PARENT_SCOPE)