/**************************************************************\
 * Reference
 * 
 * https://loup-vaillant.fr/tutorials/chacha20-design
 * https://cr.yp.to/chacha/chacha-20080128.pdf
 * https://datatracker.ietf.org/doc/html/rfc7539#section-2.3
 * https://android.googlesource.com/kernel/common/+/1855aaccd74c/lib/chacha20.c
 */

#include "chacha20.h"

// left rotation
static inline void rotl32(uint32_t *value, int n) {
    *value = (*value << n) | (*value >> (32 - n));
}

void ChaCha_Quarter_Round(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
    /* 
    If the data type of a,b,c,d is greater than 32 bits, 
    you must add a bitwise operation "& 0xffffffff" or "% 2^32"
    to the addition operation to perform the addition operation with a 32-bit module.
    */
    *a += *b;  *d ^= *a;  rotl32(d, 16);
    *c += *d;  *b ^= *c;  rotl32(b, 12);
    *a += *b;  *d ^= *a;  rotl32(d, 8);
    *c += *d;  *b ^= *c;  rotl32(b, 7);
}

static inline void chacha20_inner_block(uint32_t *state) {
    for (int i = 0; i < 10; i++) { // 20 rounds, 2 rounds per loop.
        ChaCha_Quarter_Round(state[0], state[4], state[ 8], state[12]); // column 0
        ChaCha_Quarter_Round(state[1], state[5], state[ 9], state[13]); // column 1
        ChaCha_Quarter_Round(state[2], state[6], state[10], state[14]); // column 2
        ChaCha_Quarter_Round(state[3], state[7], state[11], state[15]); // column 3
        ChaCha_Quarter_Round(state[0], state[5], state[10], state[15]); // diagonal 1
        ChaCha_Quarter_Round(state[1], state[6], state[11], state[12]); // diagonal 2
        ChaCha_Quarter_Round(state[2], state[7], state[ 8], state[13]); // diagonal 3
        ChaCha_Quarter_Round(state[3], state[4], state[ 9], state[14]); // diagonal 4
    }
}

void chacha20_block(uint32_t *block) {
    uint32_t tmp[16];
    for (int i = 0; i < 16; i++) {
        tmp[i] = block[i];
    }

    // Scramble the block
    chacha20_inner_block(tmp);

    // Add the unscrambled block to the scrambled block
    for (int i = 0; i < 16; i++) {
        block[i] += tmp[i];
    }

    // Increasing block counter
    //block[12]++;
}   