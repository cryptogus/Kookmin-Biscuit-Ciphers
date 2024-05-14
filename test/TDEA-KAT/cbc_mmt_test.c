/**
 * @cbc_mmt_test.c
 * null padding version
*/

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "tdes.h"

int main() {
    FILE *fp_ecb = fopen("TDES_CBC_MMT.nif", "w");

    FILE* ecb = fopen("TCBCMMT3.rsp", "r");

    unsigned char tkey[24];
    unsigned char IV[8];
    unsigned char plaintext[128];
    unsigned char ciphertext[128];


    if (fp_ecb == NULL || ecb == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }
    fprintf(fp_ecb,"# CAVS 18.0 for NIF\n");
    fprintf(fp_ecb,"# Config Info for : \"TDESTestVectors\"\n");
    fprintf(fp_ecb,"# TDES Multi block Message Test for CBC\n");
    fprintf(fp_ecb,"# State : Encrypt and Decrypt\n");
    time_t current_time;
    time(&current_time);
    struct tm *timeinfo = localtime(&current_time);

    fprintf(fp_ecb,"# %04d-%02d-%02d %02d:%02d:%02d\n\n",
            timeinfo->tm_year + 1900,   // 년도
            timeinfo->tm_mon + 1,       // 월
            timeinfo->tm_mday,          // 일
            timeinfo->tm_hour,          // 시
            timeinfo->tm_min,           // 분
            timeinfo->tm_sec);          // 초

    fseek(ecb, 0xb7, SEEK_SET);
    fprintf(fp_ecb, "[ENCRYPT]\n\n");

    for (int i = 0; i < 10; i++) {
        fprintf(fp_ecb, "COUNT = %d\n", i);

        fprintf(fp_ecb, "KEY1 = ");
        for (int j = 0; j < 8; j++)
			fscanf(ecb, "%2hhx", &tkey[j]); // 2자리씩 hex값 읽어오기
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");

        fseek(ecb, 9, SEEK_CUR);
        fprintf(fp_ecb, "KEY2 = ");
        for (int j = 8; j < 16; j++)
			fscanf(ecb, "%2hhx", &tkey[j]);
        for (int j = 8; j < 16; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");

        fseek(ecb, 9, SEEK_CUR);
        fprintf(fp_ecb, "KEY3 = ");
        for (int j = 16; j < 24; j++)
			fscanf(ecb, "%2hhx", &tkey[j]);
        for (int j = 16; j < 24; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");

        fseek(ecb, 7, SEEK_CUR);
        fprintf(fp_ecb, "IV = ");
        for (int j = 0; j < 8; j++)
			fscanf(ecb, "%2hhx", &IV[j]);
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", IV[j]);
        fprintf(fp_ecb, "\n");

        fseek(ecb, 14, SEEK_CUR);
        fprintf(fp_ecb, "PLAINTEXT = ");
        for (int j = 0; j < 8 + 8*i; j++)
			fscanf(ecb, "%2hhx", &plaintext[j]);
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_ecb, "%02x", plaintext[j]);
        fprintf(fp_ecb, "\n");
        crypto_const_byte_buf_t cipher_input_n = {.data = plaintext, .len = 8 + 8*i};
        crypto_byte_buf_t cipher_output_n = {.data = ciphertext, .len = 8 + 8*i};

        fprintf(fp_ecb, "CIPHERTEXT = ");
        if (nifcrypto_des(&key, iv, kBlockCipherModeCbc, kDesOperationEncrypt, cipher_input_n, kDesPaddingNull, &cipher_output_n) != kCryptoStatusOK) {
            fprintf(stderr, "nif des fail\n");
            return -1;
        }
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_ecb, "%02x", ciphertext[j]);
        fprintf(fp_ecb, "\n\n");

        fseek(ecb, 16*i + 53, SEEK_CUR);

    }
    fseek(ecb, 13, SEEK_CUR);
    fprintf(fp_ecb, "[DECRYPT]\n\n");

    for (int i = 0; i < 10; i++) {
        fprintf(fp_ecb, "COUNT = %d\n", i);

        fprintf(fp_ecb, "KEY1 = ");
        for (int j = 0; j < 8; j++)
			fscanf(ecb, "%2hhx", &tkey[j]); // 2자리씩 hex값 읽어오기
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");

        fseek(ecb, 9, SEEK_CUR);
        fprintf(fp_ecb, "KEY2 = ");
        for (int j = 8; j < 16; j++)
			fscanf(ecb, "%2hhx", &tkey[j]);
        for (int j = 8; j < 16; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");

        fseek(ecb, 9, SEEK_CUR);
        fprintf(fp_ecb, "KEY3 = ");
        for (int j = 16; j < 24; j++)
			fscanf(ecb, "%2hhx", &tkey[j]);
        for (int j = 16; j < 24; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");

        fseek(ecb, 7, SEEK_CUR);
        fprintf(fp_ecb, "IV = ");
        for (int j = 0; j < 8; j++)
			fscanf(ecb, "%2hhx", &IV[j]);
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", IV[j]);
        fprintf(fp_ecb, "\n");

        fseek(ecb, 14, SEEK_CUR);
        fprintf(fp_ecb, "CIPHERTEXT = ");
        for (int j = 0; j < 8 + 8*i; j++)
			fscanf(ecb, "%2hhx", &plaintext[j]);
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_ecb, "%02x", plaintext[j]);
        fprintf(fp_ecb, "\n");
        crypto_const_byte_buf_t cipher_input_n = {.data = plaintext, .len = 8 + 8*i};
        crypto_byte_buf_t cipher_output_n = {.data = ciphertext, .len = 8 + 8*i};

        fprintf(fp_ecb, "PLAINTEXT = ");
        if (nifcrypto_des(&key, iv, kBlockCipherModeCbc, kDesOperationDecrypt, cipher_input_n, kDesPaddingNull, &cipher_output_n) != kCryptoStatusOK) {
            fprintf(stderr, "nif des fail\n");
            return -1;
        }
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_ecb, "%02x", ciphertext[j]);
        fprintf(fp_ecb, "\n\n");

        fseek(ecb, 16*i + 52, SEEK_CUR);

    }

}
