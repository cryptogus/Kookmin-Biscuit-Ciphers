#include "ECB.h"

//ECB운용모드
void ECB(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des) {

    uint8_t key2[16];

    for (size_t i = 0; i < len/block_size; i++)
    {
        //AES 키 바이트 16 -> 만약 AES key 크기가 달라지면 16을 바꿔야함
        memcpy(key2, key, 16); // key 변수만 사용하면 on-the-fly로 encryption 코드를 짜서 key값이 계속 갱신되어 마스터키 값이 달라짐
        cipher(des + (i << 4) ,src + (i << 4), key2);
    }
}
