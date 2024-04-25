#include <stdio.h>
#include "iso9797m2.h"

// ISO9797M2 패딩 함수
unsigned char *ios9797m2_padding(unsigned char *input, size_t block_size, size_t *len) {
    
    // 패딩 값 계산
    size_t padding_value = block_size - (strlen(input) % block_size);
    
    // block 크기에 입력값이 딱 맞는 경우
    if (padding_value == block_size)
        padding_value = 0;
    
    *len = strlen(input) + padding_value;
    
    // free 필요
    unsigned char *result = (unsigned char *)malloc(sizeof(unsigned char)* (*len));

    if (result == NULL) {
        fprintf(stderr, "memory allocation fail\n");
        return NULL;
    }

    strcpy(result, input);
    memset(result + strlen(input), 0x0, padding_value);
    result[strlen(input)] = 0x80;

    return result;
}

// ISO9797M2 패딩 제거를 위한 함수
unsigned char *ios9797m2_depadding(unsigned char *input, size_t block_size, size_t *len) {
    
    // 패딩 값 계산
    size_t padding_value = input[*len - 1];
    
    // free 필요
    unsigned char *result = (unsigned char *)malloc(sizeof(unsigned char)* (*len + 1));
    int i, cnt = 1;
    for (i = 1; i <= 8; i++) {
          if (input[*len - i] == 0x80) {  
                *len -= cnt;
                break;
          }else if (input[*len - i] == 0) {
                cnt++;
            if (cnt == block_size + 1) { // 8 or 16 bytes that are zero -> error// 만약 16 byte 크기 block이라면 패딩 길이가 16을 넘길 수 없다. des나 pipo같은 암호도 8 byte를 넘길 수 없다.
                fprintf(stderr, "ISO97979M2 depadding error, input string is strange\n");
                break;  
            }
          }
          else {// Incorrect padding
            fprintf(stderr, "ISO97979M2 depadding error, input string is not ISO97979M2 padding\n");
            break;
          }
        }
    
    
    if (result == NULL) {
        fprintf(stderr, "memory allocation fail\n");
        return NULL;
    }

    memcpy(result, input, *len);
    
    // 문자열로 복구
    result[*len] = '\0';
    
    return result;
}