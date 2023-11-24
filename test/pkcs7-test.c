#include <stdio.h>
#include "pkcs7.h"

// 32 바이트 이하 문자열로 padding 테스트
void usage()
{
    printf("pkcs7-test <32 byte이하 text>\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {   
        usage();
        return -1;
    }

    // padding된 결과 배열의 길이
    size_t len = 0;
    unsigned char *pad_text = pkcs7_padding(argv[1], 16, &len);
    
    for (int i = 0; i < len; i++)
    {
        printf("%02x ", pad_text[i]);
    }
    free(pad_text);
}