cmake_minimum_required(VERSION 3.10)

project(nif_tdes LANGUAGES C)

include_directories(include/)

add_executable(ecb_mmt_test ecb_mmt_test.c tdes.c nif_tdes.c integrity.c hardened.c)
target_compile_options(ecb_mmt_test PRIVATE -g)

add_executable(cbc_mmt_test cbc_mmt_test.c tdes.c nif_tdes.c integrity.c hardened.c)
target_compile_options(cbc_mmt_test PRIVATE -g)

add_executable(ctr_mmt_test ctr_mmt_test.c tdes.c nif_tdes.c integrity.c hardened.c)
target_compile_options(ctr_mmt_test PRIVATE -g)

add_executable(ecb_mct_test ecb_mct_test.c tdes.c nif_tdes.c integrity.c hardened.c)

add_executable(cbc_mct_test cbc_mct_test.c tdes.c nif_tdes.c integrity.c hardened.c)

# parity bit check
add_executable(ecb_mct_parity_test ecb_mct_parity_test.c tdes.c nif_tdes.c integrity.c hardened.c)
add_executable(cbc_mct_parity_test cbc_mct_parity_test.c tdes.c nif_tdes.c integrity.c hardened.c)

#add_executable(cbc_mct_test cbc_mct_test.c tdes.c nif_tdes.c integrity.c hardened.c)
#target_compile_options(cbc_mct_test PRIVATE -g)
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
