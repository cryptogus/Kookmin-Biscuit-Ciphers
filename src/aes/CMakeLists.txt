set(AES128 "")

# 목적파일 생성
add_library(aes128 OBJECT aes128.c)
target_include_directories(aes128 PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set(AES128 ${AES128} $<TARGET_OBJECTS:aes128>)

# 부모 cmake에 자식 변수 알려주기
set(AES128 ${AES128} PARENT_SCOPE)