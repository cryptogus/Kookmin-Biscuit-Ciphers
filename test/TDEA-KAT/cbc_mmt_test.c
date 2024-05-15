/**
 * @cbc_mmt_test.c
 * null padding version
*/

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "tdes.h"

int main() {
    FILE *fp_cbc = fopen("TDES_CBC_MMT.rsp", "w");

    FILE* cbc = fopen("TCBCMMT3.rsp", "r");

    unsigned char tkey[24];
    unsigned char IV[8];
    unsigned char plaintext[128];
    unsigned char ciphertext[128];


    if (fp_cbc == NULL || cbc == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }
    fprintf(fp_cbc,"# CAVS 18.0 for my TDEA\n");
    fprintf(fp_cbc,"# Config Info for : \"TDESTestVectors\"\n");
    fprintf(fp_cbc,"# TDES Multi block Message Test for CBC\n");
    fprintf(fp_cbc,"# State : Encrypt and Decrypt\n");
    time_t current_time;
    time(&current_time);
    struct tm *timeinfo = localtime(&current_time);

    fprintf(fp_cbc,"# %04d-%02d-%02d %02d:%02d:%02d\n\n",
            timeinfo->tm_year + 1900,   // 년도
            timeinfo->tm_mon + 1,       // 월
            timeinfo->tm_mday,          // 일
            timeinfo->tm_hour,          // 시
            timeinfo->tm_min,           // 분
            timeinfo->tm_sec);          // 초

    fseek(cbc, 0xb7, SEEK_SET);
    fprintf(fp_cbc, "[ENCRYPT]\n\n");

    for (int i = 0; i < 10; i++) {
        fprintf(fp_cbc, "COUNT = %d\n", i);

        fprintf(fp_cbc, "KEY1 = ");
        for (int j = 0; j < 8; j++)
			fscanf(cbc, "%2hhx", &tkey[j]); // 2자리씩 hex값 읽어오기
        for (int j = 0; j < 8; j++)
            fprintf(fp_cbc, "%02x", tkey[j]);
        fprintf(fp_cbc, "\n");

        fseek(cbc, 9, SEEK_CUR);
        fprintf(fp_cbc, "KEY2 = ");
        for (int j = 8; j < 16; j++)
			fscanf(cbc, "%2hhx", &tkey[j]);
        for (int j = 8; j < 16; j++)
            fprintf(fp_cbc, "%02x", tkey[j]);
        fprintf(fp_cbc, "\n");

        fseek(cbc, 9, SEEK_CUR);
        fprintf(fp_cbc, "KEY3 = ");
        for (int j = 16; j < 24; j++)
			fscanf(cbc, "%2hhx", &tkey[j]);
        for (int j = 16; j < 24; j++)
            fprintf(fp_cbc, "%02x", tkey[j]);
        fprintf(fp_cbc, "\n");

        TDES_CTX ctx;
        TDES_set_key(&ctx, (uint32_t *)tkey, 24);

        fseek(cbc, 7, SEEK_CUR);
        fprintf(fp_cbc, "IV = ");
        for (int j = 0; j < 8; j++)
			fscanf(cbc, "%2hhx", &IV[j]);
        for (int j = 0; j < 8; j++)
            fprintf(fp_cbc, "%02x", IV[j]);
        fprintf(fp_cbc, "\n");
        ctx.IV = (uint32_t *)IV;

        fseek(cbc, 14, SEEK_CUR);
        fprintf(fp_cbc, "PLAINTEXT = ");
        for (int j = 0; j < 8 + 8*i; j++)
			fscanf(cbc, "%2hhx", &plaintext[j]);
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_cbc, "%02x", plaintext[j]);
        fprintf(fp_cbc, "\n");

        fprintf(fp_cbc, "CIPHERTEXT = ");
        if (TDES_CBC_Enc(&ctx, (uint32_t *)ciphertext, (uint32_t *)plaintext, 8 + 8*i) != 1) {
            fprintf(stderr, "TDEA MMT Fail(ENC)\n");
            return -1;
        }
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_cbc, "%02x", ciphertext[j]);
        fprintf(fp_cbc, "\n\n");

        fseek(cbc, 16*i + 53, SEEK_CUR);

    }
    fseek(cbc, 13, SEEK_CUR);
    fprintf(fp_cbc, "[DECRYPT]\n\n");

    for (int i = 0; i < 10; i++) {
        fprintf(fp_cbc, "COUNT = %d\n", i);

        fprintf(fp_cbc, "KEY1 = ");
        for (int j = 0; j < 8; j++)
			fscanf(cbc, "%2hhx", &tkey[j]); // 2자리씩 hex값 읽어오기
        for (int j = 0; j < 8; j++)
            fprintf(fp_cbc, "%02x", tkey[j]);
        fprintf(fp_cbc, "\n");

        fseek(cbc, 9, SEEK_CUR);
        fprintf(fp_cbc, "KEY2 = ");
        for (int j = 8; j < 16; j++)
			fscanf(cbc, "%2hhx", &tkey[j]);
        for (int j = 8; j < 16; j++)
            fprintf(fp_cbc, "%02x", tkey[j]);
        fprintf(fp_cbc, "\n");

        fseek(cbc, 9, SEEK_CUR);
        fprintf(fp_cbc, "KEY3 = ");
        for (int j = 16; j < 24; j++)
			fscanf(cbc, "%2hhx", &tkey[j]);
        for (int j = 16; j < 24; j++)
            fprintf(fp_cbc, "%02x", tkey[j]);
        fprintf(fp_cbc, "\n");

        TDES_CTX ctx;
        TDES_set_key(&ctx, (uint32_t *)tkey, 24);

        fseek(cbc, 7, SEEK_CUR);
        fprintf(fp_cbc, "IV = ");
        for (int j = 0; j < 8; j++)
			fscanf(cbc, "%2hhx", &IV[j]);
        for (int j = 0; j < 8; j++)
            fprintf(fp_cbc, "%02x", IV[j]);
        fprintf(fp_cbc, "\n");
        ctx.IV = (uint32_t *)IV;

        fseek(cbc, 14, SEEK_CUR);
        fprintf(fp_cbc, "CIPHERTEXT = ");
        for (int j = 0; j < 8 + 8*i; j++)
			fscanf(cbc, "%2hhx", &plaintext[j]);
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_cbc, "%02x", plaintext[j]);
        fprintf(fp_cbc, "\n");

        fprintf(fp_cbc, "PLAINTEXT = ");
        if (TDES_CBC_Dec(&ctx, (uint32_t *)ciphertext, (uint32_t *)plaintext, 8 + 8*i) != 1) {
            fprintf(stderr, "TDEA MMT Fail(ENC)\n");
            return -1;
        }
        for (int j = 0; j < 8 + 8*i; j++)
            fprintf(fp_cbc, "%02x", ciphertext[j]);
        fprintf(fp_cbc, "\n\n");

        fseek(cbc, 16*i + 52, SEEK_CUR);

    }

}
