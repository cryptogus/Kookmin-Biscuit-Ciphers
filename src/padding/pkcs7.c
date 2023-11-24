#include "pkcs7.h"
#include <stdio.h>

// PKCS#7 패딩 함수
unsigned char *pkcs7_padding(unsigned char *input, size_t block_size, size_t *len) {
    
    // 패딩 값 계산
    size_t padding_value = block_size - (strlen(input) % block_size);
    
    // block 크기에 입력값이 딱 맞는 경우
    if (padding_value == 16)
        padding_value = 0;
    
    *len = strlen(input) + padding_value;
    
    // free 필요
    unsigned char *result = (unsigned char *)malloc(sizeof(unsigned char)* (*len));

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

// PKCS#7 패딩 함수
unsigned char *pkcs7_depadding(unsigned char *input, size_t block_size, size_t len) {
    
    // 패딩 값 계산
    size_t padding_value = input[sizeof(input)/sizeof(unsigned char) - 1];

    // free 필요
    unsigned char *result = (unsigned char *)malloc(sizeof(unsigned char)* (len));

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