#ifndef QTAPI_H
#define QTAPI_H

/*
라이브러리 링크 문제:

g++로 컴파일할 때는 C++ 런타임 라이브러리가 필요할 수 있습니다. 필요한 경우 -lstdc++ 플래그를 사용하여 링크해보세요.
C++ 코드에서 C 라이브러리를 사용하는 경우, extern "C" 블록으로 감싸진 헤더 파일을 사용하는 것이 중요합니다. 이것은 C++ 이름 맹글링과 관련된 문제를 해결할 수 있습니다.

// 예: mylibrary.h
#ifdef __cplusplus
extern "C" {
#endif

// 라이브러리 관련 코드...

#ifdef __cplusplus
}
#endif

*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

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
#ifdef __cplusplus
}
#endif
#endif