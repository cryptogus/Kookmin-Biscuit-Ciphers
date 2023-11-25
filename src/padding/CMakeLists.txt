set(PADDING "")

# 목적파일 생성
add_library(padding OBJECT pkcs7.c)
target_include_directories(padding PRIVATE ${CMAKE_CURRENT_LIST_DIR})
#[[
-fPIC는 Position Independent Code(좌표 독립 코드)의 약어로, 컴파일러에게 생성되는 코드가 메모리에서 어디에나 위치할 수 있도록 하는 옵션입니다. 주로 공유 라이브러리를 만들 때 사용됩니다.

-fPIC을 사용하면 코드 섹션의 주소가 런타임에 결정되기 때문에 공유 라이브러리를 여러 응용 프로그램에서 공유할 때 유용합니다. 공유 라이브러리를 만들 때는 -fPIC 또는 -fpic 옵션을 사용하여 Position Independent Code를 생성해야 합니다.
]]
target_compile_options(padding PRIVATE -fPIC)
set(PADDING ${PADDING} $<TARGET_OBJECTS:padding>)

# 부모 cmake에 자식 변수 알려주기
set(PADDING ${PADDING} PARENT_SCOPE)
