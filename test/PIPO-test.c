#include <stdio.h>
#include "pipo.h"

u32 MASTER_KEY[2 * INT_NUM] = { 0, };
u32 ROUND_KEY[(13 + 1) * INT_NUM] = { 0, };
u32 PLAIN_TEXT[SIZE] = { 0, };
u32 CIPHER_TEXT[SIZE] = { 0, };

int main(void)
{
    unsigned char *plainText;
	unsigned char *enc_key;
    unsigned char *dec_key;
    unsigned char *cipherText;
	
    //PIPO-64/128,PIPO-64/256 test vector
    PLAIN_TEXT[0] = 0x1E270026;
	PLAIN_TEXT[1] = 0x098552F6;
    
    // PIPO-64/128 test vector
    // MASTER_KEY[0] = 0x2E152297;
	// MASTER_KEY[1] = 0x7E1D20AD;
	// MASTER_KEY[2] = 0x779428D2;
	// MASTER_KEY[3] = 0x6DC416DD;

    // PIPO-64/256 test vector
	MASTER_KEY[7] = 0x009A3AA4;
	MASTER_KEY[6] = 0x76A96DB5;
	MASTER_KEY[5] = 0x54A71206;
	MASTER_KEY[4] = 0x26D15633;
	MASTER_KEY[3] = 0x6DC416DD;
	MASTER_KEY[2] = 0x779428D2;
	MASTER_KEY[1] = 0x7E1D20AD;
	MASTER_KEY[0] = 0x2E152297;

    // 구현한 PIPO의 암복호화 결과를 담을 배열
    unsigned char usr_plainText[8] = {0,};
    unsigned char usr_cipherText[8] = {0,};
    plainText = (u8*)PLAIN_TEXT;

    printf("[ plain text ] - ");
	for (int i = 0; i < 8; i++) {
		printf("%02x ", plainText[i]);
	}
	printf("\n\n");
    
    enc_key = (u8*)MASTER_KEY;
    dec_key = (u8*)MASTER_KEY;
	printf("[     key    ] - ");
	for (int i = 0; i < 16; i++) {
		printf("%02x ", enc_key[i]);
	}
	printf("\n\n=================================================================\n");

    PIPO256_ENC((u32*)usr_cipherText, (u32*)plainText, MASTER_KEY);

    printf("PIPO128 Encryption success\n\n");
    printf("Encryption result: ");
    for (int i = 0; i < 8; i++) {
		printf("%02x ", usr_cipherText[i]);
	}
    printf("\n=================================================================\n");
    PIPO256_DEC((u32*)usr_plainText, (u32*)usr_cipherText, MASTER_KEY);

    printf("PIPO128 Decryption success\n\n");
    printf("Decryption result: ");
    for (int i = 0; i < 8; i++) {
		printf("%02x ", usr_plainText[i]);
	}
    printf("\n=================================================================\n");
    return 0;
}