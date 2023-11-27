set(PIPO "")

# 목적파일 생성
add_library(pipo OBJECT pipo.c)
target_include_directories(pipo PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set(PIPO ${PIPO} $<TARGET_OBJECTS:pipo>)

# 부모 cmake에 자식 변수 알려주기
set(PIPO ${PIPO} PARENT_SCOPE)