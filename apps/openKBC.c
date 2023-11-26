#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"

// 현재는 padding 방법을 pkcs7으로 통일, 선택권 X
void usage()
{
    printf("Usage: openKBC [options] <cipher> <mode> <plaintext or ciphertext> <key> <iv>...\n");
    printf("Sample: openKBC -e aes128 ecb \"With great power comes great responsibility\" spiderman0000000 0\n\n");
    printf("-h is help option\n");
}

void help()
{
    usage();
    printf("\n");
    printf("Options:\n");
    printf("-e encrypt\n");
    printf("-d decrypt\n\n");
    
    printf("ciphers:\n");
    printf("aes128\t");

    printf("\n\n");
    printf("modes of operation:\n");
    printf("ecb\tcbc\n");

}


int main(int argc, char *argv[]) {
    
    /* 
    FILE *inputFile, *outputFile;
    inputFile = fopen(inputFileName, "rb");
    outputFile = fopen(outputFileName, "wb");
    

    if (inputFile == NULL || outputFile == NULL) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }*/
    if (strcmp(argv[1], "-h") == 0 && argc == 2) {
        help();
        return -1;
    }
    if (argc < 6) {
        usage();
        return 1;
    }
    if (strcmp(argv[1], "-e") == 0) {
        goto enc;

    }else if (strcmp(argv[1], "-d") == 0) {
        goto dec;

    }else{
        usage();
        return 2;
    }

    enc:
        // 함수 포인터로 필요한 block cipher 함수 받아옴
        void (*cipher)(uint8_t *, uint8_t *, uint8_t *);
        
        // pkcs7 padding
        size_t len = 0;
        unsigned char *pad_plainText = pkcs7_padding(argv[4], BLOCK_SIZE, &len);
        unsigned char *cipherText = calloc(sizeof(unsigned char), len);

        if (strcmp(argv[2], "aes128") == 0){
            cipher = AES128_Encrypt;
        }else{
            usage();
            return 3;
        }

        if (strcmp(argv[3], "ecb") == 0){
            // argv[5]에 16자리 문자열을 입력하면 실제 메모리에는 '\0' 포함 17-byte를 사용하지만 ECB 함수에서는 새로운 변수에 키 길이만큼(strlen 사용)만 사용하기에 그대로 인자로 넘겨주기에 문제 없음
            ECB(cipher, argv[5],BLOCK_SIZE, len, pad_plainText, cipherText);

        }else if (strcmp(argv[3], "cbc") == 0){
            CBC_enc(cipher, argv[6], argv[5],BLOCK_SIZE, len, pad_plainText, cipherText);
        }else {
            usage();
            return 4;
        }

        printf("ciphertext: ");
        for (size_t i = 0; i < len; i++){
            printf("%02x", cipherText[i]);
        }
        printf("\n");
        free(pad_plainText);
        free(cipherText);

    dec:

    return 0;
}