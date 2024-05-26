#include "aes.h"
#include <stdio.h>
#include <string.h>

void AES_ECB_MCT(int enc, uint8_t *state ,uint8_t *key, size_t key_len) {

    int i, j;
    char name[19] = "AES_MCT_ECB";

    FILE* ecb;
    if (key_len == 128) {
        ecb = fopen("ECBMCT128.rsp", "r");
        strcat(name, "128.rsp");
    }
    else if (key_len == 192) {
        ecb = fopen("ECBMCT192.rsp", "r");
        strcat(name, "192.rsp");
    }
    else {
        ecb = fopen("ECBMCT256.rsp", "r");
        strcat(name, "256.rsp");
    }

    FILE *fp_ecb = fopen(name, "w");
    uint8_t state_tmp[16];
    for (i = 0; i < 100; i++) {
        // output NEW KEY
        // output NEW PLAINTEXT or CIPHERTEXT
        fprintf(fp_ecb,"COUNT = %d\n",i);
        fprintf(fp_ecb,"KEY = ");
        for (int k = 0; k < key_len/8; k++) {
            fprintf(fp_ecb,"%02x", key[k]);
        }
        fprintf(fp_ecb,"\n");
        if (enc == 0)
            fprintf(fp_ecb,"PLAINTEXT = ");
        else
            fprintf(fp_ecb,"CIPHERTEXT = ");
        for (int k = 0; k < 16; k++) {
            fprintf(fp_ecb,"%02x", state[k]);
        }
        fprintf(fp_ecb,"\n");

        for (j = 0; j < 1000; j++) {
            // aes input, 동작 -> output
            // NEW input = AES의 output
            if (j == 999)
                memcpy(state_tmp, state, 16);

            uint8_t key_tmp[key_len/8];
            memcpy(key_tmp, key, key_len/8);
            if (enc == 0) {
                if (key_len == 128)
                    AES128_Encrypt(state, state, key_tmp);
                else if (key_len == 192)
                    AES192_Encrypt(state, state, key_tmp);
                else
                    AES256_Encrypt(state, state, key_tmp);
            }else {
                if (key_len == 128)
                    AES128_Decrypt(state, state, key_tmp);
                else if (key_len == 192)
                    AES192_Decrypt(state, state, key_tmp);
                else
                    AES256_Decrypt(state, state, key_tmp);
            }
        }
        if (enc == 0)
            fprintf(fp_ecb,"CIPHERTEXT = ");
        else
            fprintf(fp_ecb,"PLAINTEXT = ");
        for (int k = 0; k < 16; k++) {
            fprintf(fp_ecb,"%02x", state[k]);
        }
        fprintf(fp_ecb,"\n\n");
        if (key_len == 128) {
            for (int k = 0; k < key_len/8; k++) {
                key[k] ^= state[k];
            }
        }else if (key_len == 192) {
            for (int k = 0; k < 8; k++)
                key[k] ^= state_tmp[k + 8]; // state_tmp[k]? // litte endian
            for (int k = 0; k < 16; k++)
                key[k + 8] ^= state[k];
        }else if (key_len == 256) {
            for (int k = 0; k < 16; k++)
                key[k] ^= state_tmp[k];
            for (int k = 16; k < 32; k++)
                key[k] ^= state[k - 16];
        }else {
            // return error
        }
    }
}

int main() {
    // ENCRYPT
    uint8_t key[16] = {0x13, 0x9a, 0x35, 0x42, 0x2f, 0x1d, 0x61, 0xde, 0x3c, 0x91, 0x78, 0x7f, 0xe0 ,0x50, 0x7a, 0xfd};
    uint8_t plaintext[16] = {0xb9, 0x14, 0x5a, 0x76, 0x8b, 0x7d, 0xc4, 0x89, 0xa0, 0x96, 0xb5, 0x46, 0xf4, 0x3b, 0x23, 0x1f};

    // DECRYPT
    uint8_t key2[16] = {0x0c, 0x60, 0xe7, 0xbf, 0x20, 0xad, 0xa9, 0xba, 0xa9, 0xe1, 0xdd, 0xf0, 0xd1, 0x54, 0x07, 0x26};
    uint8_t ciphertext[16] = {0xb0, 0x8a, 0x29, 0xb1, 0x1a, 0x50, 0x0e, 0xa3, 0xac, 0xa4, 0x2c, 0x36, 0x67, 0x5b, 0x97, 0x85};
    // AES_ECB_MCT(0, plaintext, key, 128);
    // fprintf(fp_ecb,"\n\n");
    // AES_ECB_MCT(1, ciphertext, key2, 128);

    // ENCRYPT
    uint8_t key_192[24] = {0xb9, 0xa6, 0x3e, 0x09, 0xe1, 0xdf, 0xc4, 0x2e, 0x93, 0xa9, 0x0d, 0x9b, 0xad, 0x73, 0x9e, 0x59, 0x67, 0xae, 0xf6, 0x72, 0xee, 0xdd, 0x5d, 0xa9};
    uint8_t plaintext_192[16] = {0x85, 0xa1, 0xf7, 0xa5, 0x81, 0x67, 0xb3, 0x89, 0xcd, 0xdc, 0x8a, 0x9f, 0xf1, 0x75, 0xee, 0x26};

    // DECRYPT
    uint8_t key2_192[24] = {0x4b, 0x97, 0x58, 0x57, 0x01, 0xc0, 0x3f, 0xbe, 0xbd, 0xfa, 0x85, 0x55, 0x02, 0x4f, 0x58, 0x9f, 0x14, 0x82, 0xc5, 0x8a, 0x00, 0xfd, 0xd9, 0xfd};
    uint8_t ciphertext_192[16] = {0xd0, 0xbd, 0x0e, 0x02, 0xde, 0xd1, 0x55, 0xe4, 0x51, 0x6b, 0xe8, 0x3f, 0x42, 0xd3, 0x47, 0xa4};

    // AES_ECB_MCT(0, plaintext_192, key_192, 192);
    // fprintf(fp_ecb,"\n\n");
    // AES_ECB_MCT(1, ciphertext_192, key2_192, 192);


    // ENCRYPT
    uint8_t key_256[32] = {0xf9, 0xe8, 0x38, 0x9f, 0x5b, 0x80, 0x71, 0x2e, 0x38, 0x86, 0xcc, 0x1f, 0xa2, 0xd2, 0x8a, 0x3b,
                            0x8c, 0x9c, 0xd8, 0x8a, 0x2d, 0x4a, 0x54, 0xc6, 0xaa, 0x86, 0xce, 0x0f, 0xef, 0x94, 0x4b, 0xe0};
    uint8_t plaintext_256[16] = {0xb3, 0x79, 0x77, 0x7f, 0x90, 0x50, 0xe2, 0xa8, 0x18, 0xf2, 0x94, 0x0c, 0xbb, 0xd9, 0xab, 0xa4};

    // DECRYPT
    uint8_t key2_256[32] = {0x2b, 0x09, 0xba, 0x39, 0xb8, 0x34, 0x06, 0x2b, 0x9e, 0x93, 0xf4, 0x83, 0x73, 0xb8, 0xdd, 0x01,
                            0x8d, 0xed, 0xf1, 0xe5, 0xba, 0x1b, 0x8a, 0xf8, 0x31, 0xeb, 0xba, 0xcb, 0xc9, 0x2a, 0x26, 0x43};
    uint8_t ciphertext_256[16] = {0x89, 0x64, 0x9b, 0xd0, 0x11, 0x5f, 0x30, 0xbd, 0x87, 0x85, 0x67, 0x61, 0x02, 0x23, 0xa5, 0x9d};

    AES_ECB_MCT(0, plaintext_256, key_256, 256);
    fprintf(fp_ecb,"\n\n");
    AES_ECB_MCT(1, ciphertext_256, key2_256, 256);
}