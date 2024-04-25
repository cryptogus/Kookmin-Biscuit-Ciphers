#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"

// 현재는 padding 방법을 pkcs7으로 통일, 선택권 X
void usage()
{
    printf("Usage: openKBC [options] <cipher> <mode> <plaintext or ciphertext> <key> <iv>...\n");
    printf("Sample1: openKBC -e aes128 ecb \"With great power comes great responsibility\" spiderman0000000 0\n");
    printf("Sample2: openKBC -e aes128 cbc \"With great power comes great responsibility\" spiderman0000000 01234567890123456\n");
#ifdef USE_OPENSSL
    printf("Sample3: openKBC rsa2048 \"OpenSSL RSA Enc Dec Test\"\n");
    printf("Sample4: openKBC -e chacha20 \"chacha20 Test\" <256-bit key> <32-bit counter> <96-bit nonce>\n\n");
#else
    printf("Sample3: openKBC -e chacha20 \"chacha20 Test\" <256-bit key> <32-bit counter> <96-bit nonce>\n\n");
#endif
    printf("-h is help option\n");
}

void help()
{
    usage();
    printf("\n");
    printf("Options:\n");
    printf("-e encrypt\n");
    printf("-d decrypt\n\n");

    printf("Block ciphers commands:\n");
    printf("aes128\taes192\taes256");

    printf("\n");
    printf("pipo64/128\tpipo64/256");

    printf("\n");
    printf("lea128\tlea192\tlea256");

    printf("\n");
    printf("aria128\taria192\taria256");

    printf("\n");
    printf("seed");

    printf("\n\n");
    printf("Stream cipher commands:\n");
    printf("chacha20");

    printf("\n\n");
    printf("Modes of operation commands:\n");
    printf("ecb\tcbc\tctr\n");
#ifdef USE_OPENSSL
    printf("\n");
    printf("Public-key cryptography:\n");
    printf("rsa2048\n");
#endif
}

// 16진수 문자열을 2바이트씩 분할하고 1바이트로 변환하는 함수
// 문자열일 때: 8d50edd3acb65e62af313df0986ac3a105cc1d264d2f51d3e2b39f6275af871985c22ac7ffb0962322c101923d4cb675
// 배열에서 1바이트로 사용할 때: 8d 50 ed d3 ac b6 5e 62 af 31 3d f0 98 6a c3 a1 05 cc 1d 26 4d 2f 51 d3 e2 b3 9f 62 75 af 87 19 85 c2 2a c7 ff b0 96 23 22 c1 01 92 3d 4c b6 75
void hexStringToBytes(const char* hexString, unsigned char* bytes, size_t length) {
    for (size_t i = 0; i < length; i++) {
        sscanf(hexString + 2 * i, "%2hhx", &bytes[i]);
    }
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
    if (argc == 1) {
        usage();
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0) {
        help();
        return -1;
    }

    if (argc < 6 && strcmp(argv[1], "rsa2048")) {
        usage();
        return 1;
    }

    if (strcmp(argv[1], "-e") == 0) {
        goto enc;

    }else if (strcmp(argv[1], "-d") == 0) {
        goto dec;
#ifdef USE_OPENSSL
    }else if (strcmp(argv[1], "rsa2048") == 0) {
        RSA *private_key = NULL;
        RSA *public_key = NULL;

        if (!generate_key_pair(&private_key, &public_key)) {
            fprintf(stderr, "Failed to generate RSA key pair.\n");
            return 1;
        }
        printf("================ Key pair ================\n");
        print_public_key(public_key);
        print_private_key(private_key);

        size_t text_length = strlen(argv[2]);

        unsigned char cipher_text[KEY_LENGTH / 8];
        char decrypted_text[KEY_LENGTH / 8];

        // 암호화
        int encrypted_length = rsa_encrypt(argv[2], text_length, public_key, cipher_text);
        if (encrypted_length == -1) {
            fprintf(stderr, "Encryption failed.\n");
            return 1;
        }

        // 복호화
        int decrypted_length = rsa_decrypt(cipher_text, encrypted_length, private_key, decrypted_text);
        if (decrypted_length == -1) {
            fprintf(stderr, "Decryption failed.\n");
            return 1;
        }

        printf("Original Text: %s\n\n", argv[2]);
        printf("Encrypted Text: ");
        for (int i = 0; i < encrypted_length; ++i) {
            printf("%02x", cipher_text[i]);
        }
        printf("\n\n");
        printf("Decrypted Text: %s\n", decrypted_text);

        // 메모리 해제
        RSA_free(private_key);
        RSA_free(public_key);

        return 2;
#endif
    }else{
        usage();
        return 2;
    }
    // 함수 포인터로 필요한 block cipher 함수 받아옴
    void (*cipher)(uint8_t *, uint8_t *, uint8_t *);

    enc:

        if (strcmp(argv[2], "aes128") == 0){
            cipher = AES128_Encrypt;
        }else if (strcmp(argv[2], "aes192") == 0){
            cipher = AES192_Encrypt;
        }else if (strcmp(argv[2], "aes256") == 0){
            cipher = AES256_Encrypt;
        }else if (strcmp(argv[2], "pipo64/128") == 0){
            // 함수 포인터 형변환
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))PIPO128_ENC;
            BLOCK_SIZE = 8;
        }else if (strcmp(argv[2], "pipo64/256") == 0){
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))PIPO256_ENC;
            BLOCK_SIZE = 8;
        }else if (strcmp(argv[2], "lea128") == 0){
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))LEA128_ENC;
        }else if (strcmp(argv[2], "lea192") == 0){
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))LEA192_ENC;
        }else if (strcmp(argv[2], "lea256") == 0){
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))LEA256_ENC;
        }else if (strcmp(argv[2], "aira128") == 0){
            cipher = ARIA128_ENC;
        }else if (strcmp(argv[2], "aira192") == 0){
            cipher = ARIA192_ENC;
        }else if (strcmp(argv[2], "aira256") == 0){
            cipher = ARIA256_ENC;
        }else if (strcmp(argv[2], "seed") == 0){
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))SEED_Enc;
        }else if (strcmp(argv[2], "chacha20") == 0) {
            u32 key[8] = {0,};
            u32 counter = 0;
            u32 nonce[3] = {0,};
            memcpy(key, argv[4], strlen(argv[4]));
            counter = (u32)atoi(argv[5]);
            memcpy(nonce, argv[6], strlen(argv[6]));

            u8 input[strlen(argv[3])]; // it only enable in linux, not in windows
            u8 output[strlen(argv[3])]; // it only enable in linux, not in windows
            memcpy(input, argv[3], strlen(argv[3]));

            chacha20_encrypt(key, counter, nonce, (u32 *)input, (u32 *)output, strlen(argv[3]));
            for (size_t i = 0; i < strlen(argv[3]); i++)
                printf("%02x", output[i]);
            printf("\n");
            return 0;
        }else{
            usage();
            return 3;
        }

        // pkcs7 padding
        size_t len = 0;
        unsigned char *pad_plainText = pkcs7_padding((unsigned char *)argv[4], BLOCK_SIZE, &len);
        unsigned char *cipherText = (unsigned char *)calloc(sizeof(unsigned char), len);

        if (strcmp(argv[3], "ecb") == 0){
            // argv[5]에 16자리 문자열을 입력하면 실제 메모리에는 '\0' 포함 17-byte를 사용하지만 ECB 함수에서는 새로운 변수에 키 길이만큼(strlen 사용)만 사용하기에 그대로 인자로 넘겨주기에 문제 없음
            ECB(cipher, (unsigned char *)argv[5],BLOCK_SIZE, len, pad_plainText, cipherText);

        }else if (strcmp(argv[3], "cbc") == 0){
            CBC_enc(cipher, (unsigned char *)argv[6], (unsigned char *)argv[5],BLOCK_SIZE, len, pad_plainText, cipherText);
        }else if (strcmp(argv[3], "ctr") == 0) {
            CTR(cipher, (unsigned char *)argv[6], (unsigned char *)argv[5],BLOCK_SIZE, len, pad_plainText, cipherText);
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

        return 0;

    dec:
        // 2바이트씩 분할한 결과를 저장할 배열
        size_t hexStringLength = strlen(argv[4]);
        size_t byteLength = hexStringLength / 2;
        unsigned char *cipherText2 = (unsigned char*)malloc(byteLength);
        unsigned char *decPlainText = (unsigned char *)calloc(sizeof(unsigned char), byteLength);

        // 16진수 문자열을 2바이트씩 분할하고 1바이트로 변환
        hexStringToBytes(argv[4], cipherText2, byteLength);

        if (strcmp(argv[2], "aes128") == 0){
            cipher = AES128_Decrypt;
        }else if (strcmp(argv[2], "aes192") == 0){
            cipher = AES192_Decrypt;
        }else if (strcmp(argv[2], "aes256") == 0){
            cipher = AES256_Decrypt;
        }else if (strcmp(argv[2], "pipo64/128") == 0){
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))PIPO128_DEC;
            BLOCK_SIZE = 8;
        }else if (strcmp(argv[2], "pipo64/256") == 0){
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))PIPO256_DEC;
            BLOCK_SIZE = 8;
        }else if (strcmp(argv[2], "lea128") == 0){
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))LEA128_DEC;
        }else if (strcmp(argv[2], "lea192") == 0){
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))LEA192_DEC;
        }else if (strcmp(argv[2], "lea256") == 0){
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))LEA256_DEC;
        }else if (strcmp(argv[2], "aira128") == 0){
            cipher = ARIA128_DEC;
        }else if (strcmp(argv[2], "aira192") == 0){
            cipher = ARIA192_DEC;
        }else if (strcmp(argv[2], "aira256") == 0){
            cipher = ARIA256_DEC;
        }else if (strcmp(argv[2], "seed") == 0){
            cipher = (void (*)(uint8_t *, uint8_t *, uint8_t *))SEED_Dec;
        }else if (strcmp(argv[2], "chacha20") == 0) {
            size_t hexStringLength_ = strlen(argv[3]);
            size_t byteLength_ = (hexStringLength_ / 2) + (hexStringLength_ % 2);

            u8 input[byteLength_]; // it only enable in linux, not in windows
            u8 output[byteLength_ + 1]; // it only enable in linux, not in windows

            // 16진수 문자열을 2바이트씩 분할하고 1바이트로 변환
            hexStringToBytes(argv[3], input, byteLength_);

            u32 key[8] = {0,};
            u32 counter = 0;
            u32 nonce[3] = {0,};
            memcpy(key, argv[4], strlen(argv[4]));
            counter = (u32)atoi(argv[5]);
            memcpy(nonce, argv[6], strlen(argv[6]));

            chacha20_encrypt(key, counter, nonce, (u32 *)input, (u32 *)output, byteLength_);
            output[byteLength_] = '\0';
            printf("%s\n", output);
            return 0;
        }else {
            usage();
            return 3;
        }

        if (strcmp(argv[3], "ecb") == 0) {
            ECB(cipher, (unsigned char *)argv[5],BLOCK_SIZE, byteLength, cipherText2, decPlainText);
        }else if (strcmp(argv[3], "cbc") == 0) {
            CBC_dec(cipher, (unsigned char *)argv[6], (unsigned char *)argv[5],BLOCK_SIZE, byteLength, cipherText2, decPlainText);
        }else if (strcmp(argv[3], "ctr") == 0) {
            CTR(cipher, (unsigned char *)argv[6], (unsigned char *)argv[5],BLOCK_SIZE, byteLength, cipherText2, decPlainText);
        }else {
            usage();
            return 4;
        }
        /*for (size_t i = 0; i < byteLength; i++){
            printf("%02x ", decPlainText[i]);
        }*/

        unsigned char *decPlainText2 = pkcs7_depadding(decPlainText, &byteLength);
        /*for (size_t i = 0; i < byteLength; i++){
            printf("%02x ", decPlainText2[i]);
        }*/
        printf("plaintext: ");
        printf("%s\n", decPlainText2);

        free(cipherText2);
        free(decPlainText);
        free(decPlainText2);

        return 0;
}