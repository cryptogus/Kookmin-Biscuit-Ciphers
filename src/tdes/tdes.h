#ifndef __TDES_H
#define __TDES_H
#pragma once
#include <stdint.h>

void keyexpansion(uint8_t *w, uint8_t *key);
void des_encrypt(uint32_t *in, uint32_t *out, uint32_t *w);
void des_decrypt(uint32_t *in, uint32_t *out, uint32_t *w);
#endif // __TDES_H