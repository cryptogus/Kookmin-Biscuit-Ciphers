#include "CBC.h"
/* 
typedef void (*cipher)(uint8_t *, uint8_t *, uint8_t *);
void CBC_enc(cipher ciphers , uint8_t *IV, uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des) {
    (*ciphers) (des + tmp ,src + tmp, key2);
}*/
 
void CBC_enc(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *IV, uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des) {

    uint8_t key2[32] = {0, };
	
	int k = 0;
    if (block_size == 16)
        k = 4;
    else if (block_size == 8)
        k = 3;
    
    for (size_t i = 0; i < len/block_size; i++)
    {
        //AES-128 키 바이트 16 -> 만약 AES key 크기가 달라지면 16을 바꿔야함, strlen(key)
        memcpy(key2, key, strlen(key)); // key 변수만 사용하면 on-the-fly로 encryption 코드를 짜서 key값이 계속 갱신되어 마스터키 값이 달라짐
        int tmp = (i << k);
        for (int j = 0; j < block_size; j++)
	    {
		    src[tmp + j] ^= IV[j];
	    }
        
        cipher(des + tmp ,src + tmp, key2);
        
        for (int j = 0; j < block_size; j++)
	    {
		    IV[j] = des[j + tmp];
	    }
    }
}

void CBC_dec(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *IV, uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des) {

    uint8_t key2[32] = {0, };
	
	int k = 0;
    if (block_size == 16)
        k = 4;
    else if (block_size == 8)
        k = 3;

    // pipo 함수 구현 방식이 src 값을 계속 바꾼 후 des에 copy하는 방식이라 값이 보존이 안되어 2번째 블록부터 (iv) XOR 연산시 문제 발생 
    // 이를 해결하기 위해 암호문을 buffer에 복사
    unsigned char *buffer;
    memcpy(buffer, src, len);
    for (size_t i = 0; i < len/block_size; i++)
    {
        
        memcpy(key2, key, strlen(key));
        
        int tmp = (i << k);
        
        cipher(des + tmp , src + tmp, key2);
        
        for (int j = 0; j < block_size; j++)
	    {
		    des[j + tmp] ^= IV[j];
            IV[j] = buffer[j + tmp];
            // pipo로 인한 갱신전: IV[j] = src[j + tmp];
	    }
    }
}