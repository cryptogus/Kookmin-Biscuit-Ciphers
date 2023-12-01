include_directories(../src/aes/ ../src/pipo/ ../src/lea ../src/seed ../src/aria ../src/padding ../src/mode)

add_executable(AES-KEY-EXPANSION-TEST AES-keyexpansion-test.c)

add_executable(aes_test ../src/aes/aes.c AES-test.c)
add_executable(pipo_test ../src/pipo/pipo.c PIPO-test.c)
add_executable(lea_test ../src/lea/lea.c LEA-test.c)
add_executable(seed_test ../src/seed/seed.c SEED-test.c)
add_executable(aria_test ../src/aria/aria.c ARIA-test.c)
add_executable(padding_test ../src/padding/pkcs7.c pkcs7-test.c)
add_executable(ECB_test ../src/padding/pkcs7.c ../src/aes/aes.c ../src/mode/ECB.c ECB-test.c)
add_executable(CBC_test ../src/padding/pkcs7.c ../src/aes/aes.c ../src/mode/CBC.c CBC-test.c)