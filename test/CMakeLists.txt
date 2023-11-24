include_directories(../src/aes/ ../src/padding)
add_executable(aes_test ../src/aes/aes128.c AES128-test.c)
add_executable(padding_test ../src/padding/pkcs7.c pkcs7-test.c)