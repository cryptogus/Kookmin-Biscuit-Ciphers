#include "pkcs7.h"
#include <stdio.h>

// PKCS#7 패딩 함수
unsigned char *pkcs7_padding(unsigned char *input, size_t block_size) {
    
    // 패딩 값 계산
    size_t padding_value = block_size - (strlen(input) % block_size);
    
    // free 필요
    unsigned char *result = (unsigned char *)malloc(sizeof(unsigned char)* (strlen(input) + padding_value));

    if (result == NULL) {
        fprintf(stderr, "memory allocation fail\n");
        return NULL;
    }

    strcpy(result, input);
    
    // 패딩 적용 & \0 제거, Decode시 문자열로 출력하려면 마지막에 \0 추가 필요
    for (size_t i = 0; i < padding_value; ++i) {
        result[strlen(input) + i] = padding_value;
    }

    return result;
}
