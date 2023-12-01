set(ARIA "")

# 목적파일 생성
add_library(aria OBJECT aria.c)
target_include_directories(aria PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set(ARIA ${ARIA} $<TARGET_OBJECTS:aria>)

# 부모 cmake에 자식 변수 알려주기
set(ARIA ${ARIA} PARENT_SCOPE)