#include <stdio.h>
#include "aria.h"

int main() {
	//test vector//
	unsigned char plaintext[16] = { 0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff };
	unsigned char key[16] = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f };
	unsigned char ciphertext[16] = { 0, };
	// expect: d718fbd6ab644c739da95f3be6451778
	unsigned char plaintext2[16] = { 0, };
	
	ARIA128_ENC(ciphertext ,plaintext, key);
	for (int i = 0; i < 16; i++)
		printf("%02x ", ciphertext[i]);
	printf("\n");

	ARIA128_DEC(plaintext2 ,ciphertext, key);
	for (int i = 0; i < 16; i++)
		printf("%02x ", plaintext2[i]);
	printf("\n");
}