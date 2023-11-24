#include <stdio.h>
#include "aes.h"

int main(void)
{
    // Test Vector
    unsigned char plainText[16] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };
	unsigned char enc_key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    unsigned char dec_key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    unsigned char cipherText[16] = { 0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32 };
	
    // 구현한 AES의 암복호화 결과를 담을 배열
    unsigned char usr_plainText[16] = {0,};
    unsigned char usr_cipherText[16] = {0,};
    
    printf("[ plain text ] - ");
	for (int i = 0; i < 16; i++) {
		printf("%02x ", plainText[i]);
	}
	printf("\n\n");
    
	printf("[     key    ] - ");
	for (int i = 0; i < 16; i++) {
		printf("%02x ", enc_key[i]);
	}
	printf("\n\n=================================================================\n");

    AES128_Encrypt(usr_cipherText, plainText, enc_key);

    for (int i = 0; i < 16; i++)
    {
        if (usr_cipherText[i] != cipherText[i])
        {
           fprintf(stderr, "AES128 Encryption fail\n");
           return -1;
        }
    }
    printf("AES128 Encryption success\n\n");
    printf("Encryption result: ");
    for (int i = 0; i < 16; i++) {
		printf("%02x ", usr_cipherText[i]);
	}
    printf("\n=================================================================\n");
    AES128_Decrypt(usr_plainText, cipherText, dec_key);

    for (int i = 0; i < 16; i++)
    {
        if (usr_plainText[i] != plainText[i])
        {
           fprintf(stderr, "AES128 Decryption fail\n");
           return -2;
        }
    }
    printf("AES128 Decryption success\n\n");
    printf("Decryption result: ");
    for (int i = 0; i < 16; i++) {
		printf("%02x ", usr_plainText[i]);
	}
    printf("\n=================================================================\n");
    return 0;
}