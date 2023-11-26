#pragma once

#include <stdint.h>

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 16	/* Change this for different block cipher */
#endif

void AES128_Encrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);
void AES128_Decrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);

unsigned char *pkcs7_padding(unsigned char *input, size_t block_size, size_t *len);
unsigned char *pkcs7_depadding(unsigned char *input, size_t *len);

void ECB(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des);

void CBC_enc(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *IV, uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des);
void CBC_dec(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *IV, uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des);