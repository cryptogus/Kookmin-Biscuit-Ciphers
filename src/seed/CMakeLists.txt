set(SEED "")

# 목적파일 생성
add_library(seed OBJECT seed.c)
target_include_directories(seed PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set(SEED ${SEED} $<TARGET_OBJECTS:seed>)

# 부모 cmake에 자식 변수 알려주기
set(SEED ${SEED} PARENT_SCOPE)