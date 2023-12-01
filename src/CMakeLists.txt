set(crypto_OBJS "")

add_subdirectory(aes)
set(crypto_OBJS ${crypto_OBJS} ${AES})
add_subdirectory(pipo)
set(crypto_OBJS ${crypto_OBJS} ${PIPO})
add_subdirectory(lea)
set(crypto_OBJS ${crypto_OBJS} ${LEA})
add_subdirectory(seed)
set(crypto_OBJS ${crypto_OBJS} ${SEED})
add_subdirectory(aria)
set(crypto_OBJS ${crypto_OBJS} ${ARIA})
add_subdirectory(mode)
set(crypto_OBJS ${crypto_OBJS} ${MODE})
add_subdirectory(padding)
set(crypto_OBJS ${crypto_OBJS} ${PADDING})

# ${PROJECT_SOURCE_DIR}
add_library(KBC SHARED ${crypto_OBJS})
# 컴파일 옵션 설정
# target_compile_options(KMS PRIVATE -fPIC)
