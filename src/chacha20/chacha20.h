#pragma once

#include <stdint.h>
void ChaCha_Quarter_Round(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);
void chacha20_block(uint32_t *block, uint32_t *output);
void chacha20_encrypt(uint32_t *key, uint32_t counter, uint32_t *nonce, uint32_t *src, uint32_t *dst, uint64_t len);