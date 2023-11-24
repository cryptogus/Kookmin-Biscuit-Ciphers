include_directories(../src/aes/ ../src/padding ../src/mode)
add_executable(aes_test ../src/aes/aes128.c AES128-test.c)
add_executable(padding_test ../src/padding/pkcs7.c pkcs7-test.c)
add_executable(ECB_test ../src/padding/pkcs7.c ../src/aes/aes128.c ../src/mode/ECB.c ECB-test.c)