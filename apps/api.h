#pragma once

#include <stdint.h>

size_t BLOCK_SIZE = 16;

void AES128_Encrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);
void AES128_Decrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

void PIPO128_ENC(u32* CIPHER_TEXT, u32* PLAIN_TEXT, u32* MASTER_KEY);
void PIPO256_ENC(u32* CIPHER_TEXT, u32* PLAIN_TEXT, u32* MASTER_KEY);
void PIPO128_DEC(u32* PLAIN_TEXT, u32* CIPHER_TEXT, u32* MASTER_KEY);
void PIPO256_DEC(u32* PLAIN_TEXT, u32* CIPHER_TEXT, u32* MASTER_KEY);

unsigned char *pkcs7_padding(unsigned char *input, size_t block_size, size_t *len);
unsigned char *pkcs7_depadding(unsigned char *input, size_t *len);

void ECB(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des);

void CBC_enc(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *IV, uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des);
void CBC_dec(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *IV, uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des);