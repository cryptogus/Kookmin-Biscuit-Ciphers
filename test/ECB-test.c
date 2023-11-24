#include <stdio.h>
#include "ECB.h"
#include "pkcs7.h"
#include "aes.h"

int main()
{
    unsigned char plainText[32] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34
                                  , 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };
	
    unsigned char key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    
    size_t len = 0;
    // padding
    unsigned char *pad_plainText = pkcs7_padding(plainText, 16, &len);
    unsigned char output[32] = {0,};

    //ECB 암호화
    ECB(AES128_Encrypt, key, 16, len, pad_plainText, output);

    for (int i = 0; i < 32; i++)
        printf("%02x ", output[i]);
    printf("\n");

    // ECB 복호화
    unsigned char output2[32] = {0,};
    ECB(AES128_Decrypt, key, 16, len, output, output2);
    
    for (int i = 0; i < 32; i++)
        printf("%02x ", output2[i]);
    printf("\n");
    
    free(pad_plainText);
}