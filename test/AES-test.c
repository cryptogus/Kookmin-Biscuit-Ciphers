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
	printf("\n\n==============================AES128 test===================================\n");

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
    printf("\n=================================================================\n\n");


    unsigned char usr_plainText2[16] = {0x6b, 0xc1, 0xbe, 0xe2,
        0x2e, 0x40, 0x9f, 0x96,
        0xe9, 0x3d, 0x7e, 0x11,
        0x73, 0x93, 0x17, 0x2a};
    uint8_t key192[24] = {
        0x8e, 0x73, 0xb0, 0xf7,
        0xda, 0x0e, 0x64, 0x52,
        0xc8, 0x10, 0xf3, 0x2b,
        0x80, 0x90, 0x79, 0xe5,
        0x62, 0xf8, 0xea, 0xd2,
        0x52, 0x2c, 0x6b, 0x7b
    };
    unsigned char usr_cipherText2[16] = {0,};
    uint8_t data[16] = {
        0xbd, 0x33, 0x4f, 0x1d,
        0x6e, 0x45, 0xf2, 0x5f,
        0xf7, 0x12, 0xa2, 0x14,
        0x57, 0x1f, 0xa5, 0xcc
    };
    // expect: bd334f1d6e45f25ff712a214571fa5cc
    
    printf("[ plain text ] - ");
	for (int i = 0; i < 16; i++) {
		printf("%02x ", usr_plainText2[i]);
	}
	printf("\n\n");
    
	printf("[     key    ] - ");
	for (int i = 0; i < 24; i++) {
		printf("%02x ", key192[i]);
	}
	printf("\n\n==============================AES192 test===================================\n");

    AES192_Encrypt(usr_cipherText2, usr_plainText2, key192);
    for (int i = 0; i < 16; i++)
    {
        if (usr_cipherText2[i] != data[i])
        {
           fprintf(stderr, "AES192 Encryption fail\n");
           return -1;
        }
    }
    printf("AES192 Encryption success\n\n");
    printf("Encryption result: ");
    for (int i = 0; i < 16; i++) {
		printf("%02x ", usr_cipherText2[i]);
	}
    printf("\n=================================================================\n");
    AES192_Decrypt(usr_plainText, usr_cipherText2, key192);

    for (int i = 0; i < 16; i++)
    {
        if (usr_plainText[i] != usr_plainText2[i])
        {
           fprintf(stderr, "AES128 Decryption fail\n");
           return -2;
        }
    }
    printf("AES192 Decryption success\n\n");
    printf("Decryption result: ");
    for (int i = 0; i < 16; i++) {
		printf("%02x ", usr_plainText[i]);
	}
    printf("\n=================================================================\n");
    return 0;
}