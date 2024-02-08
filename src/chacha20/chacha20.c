/**************************************************************\
 * Reference
 * 
 * https://loup-vaillant.fr/tutorials/chacha20-design
 * https://cr.yp.to/chacha/chacha-20080128.pdf
 * https://datatracker.ietf.org/doc/html/rfc7539#section-2.3
 * 
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