set(RSA2048 "")

# 목적파일 생성
add_library(rsa2048 OBJECT rsa2048.c)
target_include_directories(rsa2048 PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set(RSA2048 ${RSA2048} $<TARGET_OBJECTS:rsa2048>)

# 부모 cmake에 자식 변수 알려주기
set(RSA2048 ${RSA2048} PARENT_SCOPE)