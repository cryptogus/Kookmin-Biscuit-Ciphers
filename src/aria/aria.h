#pragma once

#include <stdint.h>

void ARIA128_ENC(uint8_t ciphertext[], uint8_t plaintext[], uint8_t key[]);
void ARIA128_DEC(uint8_t plaintext[], uint8_t ciphertext[], uint8_t key[]);

void ARIA192_ENC(uint8_t ciphertext[], uint8_t plaintext[], uint8_t key[]);
void ARIA192_DEC(uint8_t plaintext[], uint8_t ciphertext[], uint8_t key[]);

void ARIA256_ENC(uint8_t ciphertext[], uint8_t plaintext[], uint8_t key[]);
void ARIA256_DEC(uint8_t plaintext[], uint8_t ciphertext[], uint8_t key[]);