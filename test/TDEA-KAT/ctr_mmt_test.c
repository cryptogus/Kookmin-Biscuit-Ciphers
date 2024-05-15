/**
 * @ctr_mmt_test.c
 * null padding version
*/

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "tdes.h"

int main() {
    FILE *fp_ctr = fopen("TDES_CTR_MMT.rsp", "w");

    FILE* ctr = fopen("TCTRMMT3.rsp", "r");

    unsigned char tkey[24];
    unsigned char IV[8];
    unsigned char plaintext[128];
    unsigned char ciphertext[128];

    if (fp_ctr == NULL || ctr == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }

    fprintf(fp_ctr,"# TDES CTR mode created by HHLEE\n");
    fprintf(fp_ctr,"# Config Info for : \"TDESTestVectors\"\n");
    fprintf(fp_ctr,"# TDES Multi block Message Test for CTR\n");
    fprintf(fp_ctr,"# State : Encrypt and Decrypt\n");
    time_t current_time;
    time(&current_time);
    struct tm *timeinfo = localtime(&current_time);

    fprintf(fp_ctr,"# %04d-%02d-%02d %02d:%02d:%02d\n\n",
            timeinfo->tm_year + 1900,   // 년도
            timeinfo->tm_mon + 1,       // 월
            timeinfo->tm_mday,          // 일
            timeinfo->tm_hour,          // 시
            timeinfo->tm_min,           // 분
            timeinfo->tm_sec);          // 초

    fseek(ctr, 0xb7, SEEK_SET);
    fprintf(fp_ctr, "[ENCRYPT]\n\n");

    for (int i = 0; i < 10; i++) {
        fprintf(fp_ctr, "COUNT = %d\n", i);

        fprintf(fp_ctr, "KEY1 = ");
        for (int j = 0; j < 8; j++)
			fscanf(ctr, "%2hhx", &tkey[j]); // 2자리씩 hex값 읽어오기
        for (int j = 0; j < 8; j++)
            fprintf(fp_ctr, "%02x", tkey[j]);
        fprintf(fp_ctr, "\n");

        fseek(ctr, 8, SEEK_CUR);
        fprintf(fp_ctr, "KEY2 = ");
        for (int j = 8; j < 16; j++)
			fscanf(ctr, "%2hhx", &tkey[j]);
        for (int j = 8; j < 16; j++)
            fprintf(fp_ctr, "%02x", tkey[j]);
        fprintf(fp_ctr, "\n");

        fseek(ctr, 8, SEEK_CUR);
        fprintf(fp_ctr, "KEY3 = ");
        for (int j = 16; j < 24; j++)
			fscanf(ctr, "%2hhx", &tkey[j]);
        for (int j = 16; j < 24; j++)
            fprintf(fp_ctr, "%02x", tkey[j]);
        fprintf(fp_ctr, "\n");

        TDES_CTX ctx;
        TDES_set_key(&ctx, (uint32_t *)tkey, 24);

        fseek(ctr, 6, SEEK_CUR);
        fprintf(fp_ctr, "IV = ");
        for (int j = 0; j < 8; j++)
			fscanf(ctr, "%2hhx", &IV[j]);
        for (int j = 0; j < 8; j++)
            fprintf(fp_ctr, "%02x", IV[j]);
        fprintf(fp_ctr, "\n");
        ctx.IV = (uint32_t *)IV;

        fseek(ctr, 13, SEEK_CUR);
        fprintf(fp_ctr, "PLAINTEXT = ");
        for (int j = 0; j < 8 + 8*i; j++)
			fscanf(ctr, "%2hhx", &plaintext[j]);
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_ctr, "%02x", plaintext[j]);
        fprintf(fp_ctr, "\n");

        fprintf(fp_ctr, "CIPHERTEXT = ");

        if (TDES_CTR(&ctx, (uint32_t *)ciphertext, (uint32_t *)plaintext, 8 + 8*i) != 1) {
            fprintf(stderr, "TDEA MMT Fail(ENC)\n");
            return -1;
        }

        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_ctr, "%02x", ciphertext[j]);
        fprintf(fp_ctr, "\n\n");

        fseek(ctr, 16*i + 49, SEEK_CUR);

    }
    fseek(ctr, 11, SEEK_CUR);
    fprintf(fp_ctr, "[DECRYPT]\n\n");

    for (int i = 0; i < 10; i++) {
        fprintf(fp_ctr, "COUNT = %d\n", i);

        fprintf(fp_ctr, "KEY1 = ");
        for (int j = 0; j < 8; j++)
			fscanf(ctr, "%2hhx", &tkey[j]); // 2자리씩 hex값 읽어오기
        for (int j = 0; j < 8; j++)
            fprintf(fp_ctr, "%02x", tkey[j]);
        fprintf(fp_ctr, "\n");

        fseek(ctr, 8, SEEK_CUR);
        fprintf(fp_ctr, "KEY2 = ");
        for (int j = 8; j < 16; j++)
			fscanf(ctr, "%2hhx", &tkey[j]);
        for (int j = 8; j < 16; j++)
            fprintf(fp_ctr, "%02x", tkey[j]);
        fprintf(fp_ctr, "\n");

        fseek(ctr, 8, SEEK_CUR);
        fprintf(fp_ctr, "KEY3 = ");
        for (int j = 16; j < 24; j++)
			fscanf(ctr, "%2hhx", &tkey[j]);
        for (int j = 16; j < 24; j++)
            fprintf(fp_ctr, "%02x", tkey[j]);
        fprintf(fp_ctr, "\n");

        TDES_CTX ctx;
        TDES_set_key(&ctx, (uint32_t *)tkey, 24);

        fseek(ctr, 6, SEEK_CUR);
        fprintf(fp_ctr, "IV = ");
        for (int j = 0; j < 8; j++)
			fscanf(ctr, "%2hhx", &IV[j]);
        for (int j = 0; j < 8; j++)
            fprintf(fp_ctr, "%02x", IV[j]);
        fprintf(fp_ctr, "\n");
        ctx.IV = (uint32_t *)IV;

        fseek(ctr, 13, SEEK_CUR);
        fprintf(fp_ctr, "CIPHERTEXT = ");
        for (int j = 0; j < 8 + 8*i; j++)
			fscanf(ctr, "%2hhx", &plaintext[j]);
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_ctr, "%02x", plaintext[j]);
        fprintf(fp_ctr, "\n");

        fprintf(fp_ctr, "PLAINTEXT = ");
        if (TDES_CTR(&ctx, (uint32_t *)ciphertext, (uint32_t *)plaintext, 8 + 8*i) != 1) {
            fprintf(stderr, "TDEA MMT Fail(DEC)\n");
            return -1;
        }
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_ctr, "%02x", ciphertext[j]);
        fprintf(fp_ctr, "\n\n");

        fseek(ctr, 16*i + 48, SEEK_CUR);

    }

}
