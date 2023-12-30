#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t BLOCK_SIZE = 16;

/***
 * AES, reference: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf
*/
void AES128_Encrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);
void AES128_Decrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);

void AES192_Encrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);
void AES192_Decrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);

void AES256_Encrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);
void AES256_Decrypt(uint8_t cipherText[16] ,uint8_t plainText[16], uint8_t key[16]);

/***
 * PIPO, reference: https://eprint.iacr.org/2020/1582.pdf
*/
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

void PIPO128_ENC(u32* CIPHER_TEXT, u32* PLAIN_TEXT, u32* MASTER_KEY);
void PIPO128_DEC(u32* PLAIN_TEXT, u32* CIPHER_TEXT, u32* MASTER_KEY);

void PIPO256_ENC(u32* CIPHER_TEXT, u32* PLAIN_TEXT, u32* MASTER_KEY);
void PIPO256_DEC(u32* PLAIN_TEXT, u32* CIPHER_TEXT, u32* MASTER_KEY);

/***
 * LEA, reference: https://seed.kisa.or.kr/kisa/algorithm/EgovLeaInfo.do
*/
void LEA128_ENC(uint32_t ciphertext[], uint32_t plaintext[], uint32_t Key[]);
void LEA128_DEC(uint32_t plaintext[], uint32_t ciphertext[], uint32_t Key[]);

void LEA192_ENC(uint32_t ciphertext[], uint32_t plaintext[], uint32_t Key[]);
void LEA192_DEC(uint32_t plaintext[], uint32_t ciphertext[], uint32_t Key[]);

void LEA256_ENC(uint32_t ciphertext[], uint32_t plaintext[], uint32_t Key[]);
void LEA256_DEC(uint32_t plaintext[], uint32_t ciphertext[], uint32_t Key[]);

/**
 * SEED, reference: https://seed.kisa.or.kr/kisa/algorithm/EgovSeedInfo.do
*/

void SEED_Enc(uint32_t* Ciphertext, uint32_t* Plaintext, uint32_t* Key);
void SEED_Dec(uint32_t* Ciphertext, uint32_t* Plaintext, uint32_t* Key);

/**
 * ARIA, reference: https://seed.kisa.or.kr/kisa/Board/19/detailView.do
*/

void ARIA128_ENC(uint8_t ciphertext[], uint8_t plaintext[], uint8_t key[]);
void ARIA128_DEC(uint8_t plaintext[], uint8_t ciphertext[], uint8_t key[]);

void ARIA192_ENC(uint8_t ciphertext[], uint8_t plaintext[], uint8_t key[]);
void ARIA192_DEC(uint8_t plaintext[], uint8_t ciphertext[], uint8_t key[]);

void ARIA256_ENC(uint8_t ciphertext[], uint8_t plaintext[], uint8_t key[]);
void ARIA256_DEC(uint8_t plaintext[], uint8_t ciphertext[], uint8_t key[]);

/**
 * padding
*/
unsigned char *pkcs7_padding(unsigned char *input, size_t block_size, size_t *len);
unsigned char *pkcs7_depadding(unsigned char *input, size_t *len);

void ECB(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des);

void CBC_enc(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *IV, uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des);
void CBC_dec(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *IV, uint8_t *key, size_t block_size, size_t len, unsigned char *src, unsigned char *des);

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/err.h>

#define KEY_LENGTH 2048

int generate_key_pair(RSA **private_key, RSA **public_key);
int rsa_encrypt(const char *plain_text, size_t plain_length, RSA *public_key, unsigned char *cipher_text);
int rsa_decrypt(const unsigned char *cipher_text, size_t cipher_length, RSA *private_key, char *decrypted_text);
void print_private_key(RSA *private_key);
void print_public_key(RSA *public_key);
int encrypt_message(const char *message, RSA *public_key, unsigned char **encrypted_message, size_t *encrypted_len);

#ifdef __cplusplus
}
#endif
