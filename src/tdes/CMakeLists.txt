set(3DES "")

# 목적파일 생성
add_library(tdes OBJECT tdes.c)
target_compile_options(tdes PRIVATE -fPIC)
target_include_directories(tdes PRIVATE ${CMAKE_CURRENT_LIST_DIR})
set(3DES ${3DES} $<TARGET_OBJECTS:tdes>)

# 부모 cmake에 자식 변수 알려주기
set(3DES ${3DES} PARENT_SCOPE)