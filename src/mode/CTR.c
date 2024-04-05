#include "CTR.h"

/**
 * @remark I just set IV == Nonce. What we write as a Nonce here is IV in other modes.
*/
void CTR(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *Nonce, uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des) {

    uint8_t key2[32] = {0, };
	
	int k = 0;
    if (block_size == 16) // 16 byte, AES, ARIA, SEED, LEA..
        k = 4;
    else if (block_size == 8) // 8 byte, PIPO
        k = 3;
    
    // initializaion Counter(0)||Nonce -> AES 기준, 총 16 byte (문서는 Nonce||Counter(0)이지만 그게 그거지 않나 싶음)
    int nonce_size = block_size >> 1;
    uint64_t counter[2] = {0, };
    memcpy(counter, Nonce, nonce_size); // get nonce
    counter[1] = 0;
    // block size가 8 바이트일 때는 사용 불가능한 코드
    // if (block_size == 8) {
    //     counter[0] >>= 32;
    //     counter[0] <<= 32;
    // }

    for (size_t i = 0; i < len/block_size; i++)
    {
        memcpy(key2, key, strlen(key));
        cipher(des +(i << k), (uint8_t *)counter, key2);
        for (int j = 0; j < block_size; j++) {
            des[(i << k) + j] ^= src[(i << k) + j];
        }
        counter[1]++;
    }
}