#include <stdio.h>
#include <stdlib.h>

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

void PIPO_ENC(u32* PLAIN_TEXT, u32* ROUND_KEY, u32* CIPHER_TEXT, int ROUND);
void PIPO_DEC(u32* CIPHER_TEXT, u32* ROUND_KEY, u32* PLAIN_TEXT, int ROUND);