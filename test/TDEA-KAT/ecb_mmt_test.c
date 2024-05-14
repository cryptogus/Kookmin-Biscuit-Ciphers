/**
 * @ecb_mmt_test.c
 * null padding version
*/

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "tdes.h"

int main() {
    FILE *fp_ecb = fopen("TDES_ECB_MMT.rsp", "w");

    FILE* ecb = fopen("TECBMMT3.rsp", "r");

    unsigned char tkey[24];
    unsigned char input[128];
    unsigned char output[128];

    if (fp_ecb == NULL || ecb == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }
    fprintf(fp_ecb,"# CAVS 18.0 for my TDEA\n");
    fprintf(fp_ecb,"# Config Info for : \"TDESTestVectors\"\n");
    fprintf(fp_ecb,"# TDES Multi block Message Test for ECB\n");
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

        TDES_CTX ctx;
        TDES_set_key(&ctx, (uint32_t *)tkey, 24);

        fseek(ecb, 14, SEEK_CUR);
        fprintf(fp_ecb, "PLAINTEXT = ");
        for (int j = 0; j < 8 + 8*i; j++)
			fscanf(ecb, "%2hhx", &input[j]);
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_ecb, "%02x", input[j]);
        fprintf(fp_ecb, "\n");

        fprintf(fp_ecb, "CIPHERTEXT = ");
        if (TDES_ECB_Enc(&ctx, (uint32_t *)output, (uint32_t *)input, 8 + 8*i) != 1) {
            fprintf(stderr, "TDEA MMT Fail(ENC)\n");
            return -1;
        }
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_ecb, "%02x", output[j]);
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

        TDES_CTX ctx;
        TDES_set_key(&ctx, (uint32_t *)tkey, 24);

        fseek(ecb, 14, SEEK_CUR);
        fprintf(fp_ecb, "CIPHERTEXT = ");
        for (int j = 0; j < 8 + 8*i; j++)
			fscanf(ecb, "%2hhx", &input[j]);
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_ecb, "%02x", input[j]);
        fprintf(fp_ecb, "\n");

        fprintf(fp_ecb, "PLAINTEXT = ");
        if (TDES_ECB_Dec(&ctx, (uint32_t *)output, (uint32_t *)input, 8 + 8*i) != 1) {
            fprintf(stderr, "TDEA MMT Fail(DEC)\n");
            return -1;
        }
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_ecb, "%02x", output[j]);
        fprintf(fp_ecb, "\n\n");

        fseek(ecb, 16*i + 52, SEEK_CUR);

    }

}
