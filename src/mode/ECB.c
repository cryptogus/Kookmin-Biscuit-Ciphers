#include "ECB.h"

//ECB운용모드
void ECB(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des) {

    // 256-bit 까지 사용가능하도록 넉넉하게 잡음
    uint8_t key2[32] = {0, };

    int k = 0;
    if (block_size == 16)
        k = 4;
    else if (block_size == 8)
        k = 3;

    for (size_t i = 0; i < len/block_size; i++)
    {
        //AES 키 바이트 16 -> strlen(key)
        memcpy(key2, key, strlen(key)); // key 변수만 사용하면 on-the-fly로 encryption 코드를 짜서 key값이 계속 갱신되어 마스터키 값이 달라짐
        cipher(des + (i << k) ,src + (i << k), key2);
    }
}
