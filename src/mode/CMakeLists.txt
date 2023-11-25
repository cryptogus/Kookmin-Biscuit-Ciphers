set(MODE "")

# 목적파일 생성
add_library(mode OBJECT ECB.c CBC.c)
target_include_directories(mode PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set(MODE ${MODE} $<TARGET_OBJECTS:mode>)

# 부모 cmake에 변수 알려주기
set(MODE ${MODE} PARENT_SCOPE)