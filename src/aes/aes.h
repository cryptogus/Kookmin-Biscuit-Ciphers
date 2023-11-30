#pragma once

#include <stdint.h>

void AES128_Encrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);
void AES128_Decrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);

void AES192_Encrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);
void AES192_Decrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);

void AES256_Encrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);
void AES256_Decrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);