set(crypto_OBJS "")

add_subdirectory(aes)
add_subdirectory(mode)

set(crypto_OBJS ${crypto_OBJS} ${AES128})

add_library(mycrypto SHARED ${crypto_OBJS})