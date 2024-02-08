#pragma once

#include <stdint.h>
void ChaCha_Quarter_Round(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);
void chacha20_block(uint32_t *block);