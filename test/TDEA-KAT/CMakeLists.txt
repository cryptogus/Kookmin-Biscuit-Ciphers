cmake_minimum_required(VERSION 3.10)

project(TDEA_TEST LANGUAGES C)

include_directories(../../src/tdes)

#######
add_executable(ecb_mmt_test ecb_mmt_test.c ../../src/tdes/tdes.c)

add_executable(cbc_mmt_test cbc_mmt_test.c ../../src/tdes/tdes.c)

add_executable(ctr_mmt_test ctr_mmt_test.c ../../src/tdes/tdes.c)
#######

add_executable(ecb_mct_test ecb_mct_test.c ../../src/tdes/tdes.c)
#add_compile_options(ecb_mct_test -O3)
add_executable(cbc_mct_test cbc_mct_test.c ../../src/tdes/tdes.c)

# key parity bit check
add_executable(ecb_mct_parity_test ecb_mct_parity_test.c ../../src/tdes/tdes.c)
#add_compile_options(ecb_mct_parity_test -O3)
add_executable(cbc_mct_parity_test cbc_mct_parity_test.c ../../src/tdes/tdes.c)

# 원본 파일의 경로
set(ORIGINAL_FILE_PATH "TECBMMT3.rsp")
set(ORIGINAL_FILE_PATH2 "TCBCMMT3.rsp")
set(ORIGINAL_FILE_PATH3 "TCTRMMT3.rsp")

set(ORIGINAL_FILE_PATH4 "TECBMonte3.rsp")
set(ORIGINAL_FILE_PATH5 "TCBCMonte3.rsp")

# 복사할 파일의 경로
set(COPIED_FILE_PATH "./")

# 파일 복사
file(COPY ${ORIGINAL_FILE_PATH} DESTINATION ${COPIED_FILE_PATH})
file(COPY ${ORIGINAL_FILE_PATH2} DESTINATION ${COPIED_FILE_PATH})
file(COPY ${ORIGINAL_FILE_PATH3} DESTINATION ${COPIED_FILE_PATH})

file(COPY ${ORIGINAL_FILE_PATH4} DESTINATION ${COPIED_FILE_PATH})
file(COPY ${ORIGINAL_FILE_PATH5} DESTINATION ${COPIED_FILE_PATH})
