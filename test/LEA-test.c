#include <stdio.h>
#include "lea.h"

int main() {

	/******************************32 - bit 단위 Test Vector*******************************/
	word Plaintext[4] = { 0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c};
	word Key[4] = { 0x3c2d1e0f, 0x78695a4b, 0xb4a59687, 0xf0e1d2c3 };
	word Ciphertext[4] = { 0, }; // 354ec89f 18c6c628 a7c73255 fd8b6404
    word uPlaintext[4] = {0, };
	/**************************************************************************************/

	/******************************8 - bit 단위 Test Vector********************************/
	byte Plaintext2[16] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
	byte Key2[16] = { 0x0f, 0x1e, 0x2d, 0x3c, 0x4b, 0x5a, 0x69, 0x78, 0x87, 0x96, 0xa5, 0xb4, 0xc3, 0xd2, 0xe1, 0xf0 };
	//byte Key2[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	byte Ciphertext2[16] = { 0, };
	/**************************************************************************************/
	
	/*******************************************TEST***************************************/
	/*LEA_Enc_32(Plaintext, Key, Ciphertext);
	printf("%08x ", Ciphertext[0]);
	printf("%08x ", Ciphertext[1]);
	printf("%08x ", Ciphertext[2]);
	printf("%08x ", Ciphertext[3]);*/
	
	LEA128_ENC(Ciphertext, Plaintext, Key);
	for (int i = 0; i < 4; i++)
		printf("%08x ", Ciphertext[i]);
    printf("\n");
    LEA128_DEC(uPlaintext, Ciphertext, Key);
	for (int i = 0; i < 4; i++)
		printf("%08x ", uPlaintext[i]);
}