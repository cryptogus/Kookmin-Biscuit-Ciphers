set(LEA "")

# 목적파일 생성
add_library(lea OBJECT lea.c)
target_include_directories(lea PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set(LEA ${LEA} $<TARGET_OBJECTS:lea>)

# 부모 cmake에 자식 변수 알려주기
set(LEA ${LEA} PARENT_SCOPE)