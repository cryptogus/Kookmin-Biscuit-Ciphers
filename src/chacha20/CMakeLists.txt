set(CHACHA20 "")

# 목적파일 생성
add_library(chacha20 OBJECT chacha20.c)
target_include_directories(chacha20 PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set(CHACHA20 ${CHACHA20} $<TARGET_OBJECTS:chacha20>)

# 부모 cmake에 자식 변수 알려주기
set(CHACHA20 ${CHACHA20} PARENT_SCOPE)