#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 2				//64 = 32 * 2
#define INT_NUM 2			//64 = 32 * 2

// PIPO64_128
// #define ROUND 13
// #define MASTER_KEY_SIZE 2	//128 = 64 * 2

// PIPO64_256
// #define ROUND 17
// #define MASTER_KEY_SIZE 4	//256 = 64 * 2


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

void PIPO128_ENC(u32* CIPHER_TEXT, u32* PLAIN_TEXT, u32* MASTER_KEY);
void PIPO256_ENC(u32* CIPHER_TEXT, u32* PLAIN_TEXT, u32* MASTER_KEY);
void PIPO128_DEC(u32* PLAIN_TEXT, u32* CIPHER_TEXT, u32* MASTER_KEY);
void PIPO256_DEC(u32* PLAIN_TEXT, u32* CIPHER_TEXT, u32* MASTER_KEY);