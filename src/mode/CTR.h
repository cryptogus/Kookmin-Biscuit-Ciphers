#ifndef __CTR_H
#define __CTR_H
/**
 * @ref https://github.com/exscape/AES/blob/master/ctr.c
 * @ref https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation
*/
#include <stdint.h>
#include <string.h>

void CTR(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *Nonce, uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des);

#endif // __CTR_H
