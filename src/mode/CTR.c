#include "CTR.h"

/**
 * @remark I just set IV == Nonce. What we write as a Nonce here is IV in other modes.
*/

/**
 * @note
 * https://crypto.stackexchange.com/questions/18267/how-do-we-compute-iv1-in-ctr-mode
 * target machine이 64-bit, little endian일 때 사용
 * CTR 모드의 counter는 반드시 메모리상에서 00 00 00 01 이런 형태로 존재해야함.
 * little endian은 uint32_t tmp = 1; 이라면, 메모리 상에는 01 00 00 00으로 존재하기 때문에 다른 머신에서는 다른 복호화 결과가 나올 수 있다.
*/

static uint64_t little_to_big_endian(uint64_t val) {
    return ((val >> 56) & 0xFF) | ((val << 40) & 0xFF000000000000) |
           ((val >> 40) & 0xFF00) | ((val << 24) & 0xFF0000000000) |
           ((val >> 24) & 0xFF0000) | ((val << 8) & 0xFF00000000) |
           ((val >> 8) & 0xFF000000) | ((val << 56) & 0xFF00000000000000);
}
/** 
 *	little endian to big endian
 *	0x01000000 -> 0x00 0x00 0x00 0x01 (little endian)
 *	0x00000001 -> 0x00 0x00 0x00 0x01 (big endian)
 *  32-bit machine, little endian

static uint32_t little_to_big_endian(uint32_t val) {
    return ((val >> 24) & 0xFF) | ((val << 8) & 0xFF0000) |
           ((val >> 8) & 0xFF00) | ((val << 24) & 0xFF000000);
}
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
    uint64_t ctr = 0;
    // block size가 8 바이트일 때는 사용 불가능한 코드
    // if (block_size == 8) {
    //     counter[0] >>= 32;
    //     counter[0] <<= 32;
    // }

    for (size_t i = 0; i < len/block_size; i++)
    {
        counter[1] = little_to_big_endian(ctr++);
        memcpy(key2, key, strlen(key));
        cipher(des +(i << k), (uint8_t *)counter, key2);
        for (int j = 0; j < block_size; j++) {
            des[(i << k) + j] ^= src[(i << k) + j];
        }
    }
}