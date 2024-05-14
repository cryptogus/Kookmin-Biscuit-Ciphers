/**
 * @ecb_mct_test.c monte carlo test
 * null padding version
 * reference: https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-20.pdf
 * The Monte Carlo Test for the Encryption Process - TECB Mode
*/

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "tdes.h"

int main() {
    FILE *fp_ecb = fopen("TDES_ECB_Monte.rsp", "w");

    FILE* ecb = fopen("TECBMonte3.rsp", "r");

    unsigned char tkey[24];
    unsigned char state[8];
    unsigned char tmp[2][8];

    if (fp_ecb == NULL || ecb == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }
    fprintf(fp_ecb,"# CAVS 11.1 for my TDEA\n");
    fprintf(fp_ecb,"# Config Info for : \"tdes_values\"\n");
    fprintf(fp_ecb,"# TDES Monte Carlo (Modes) Test for ECB\n");
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

    fseek(ecb, 0xb2, SEEK_SET);
    for (int j = 0; j < 8; j++)
		fscanf(ecb, "%2hhx", &tkey[j]); // 2자리씩 hex값 읽어오기
    fseek(ecb, 9, SEEK_CUR);
    for (int j = 8; j < 16; j++)
		fscanf(ecb, "%2hhx", &tkey[j]);
    fseek(ecb, 9, SEEK_CUR);
    for (int j = 16; j < 24; j++)
		fscanf(ecb, "%2hhx", &tkey[j]);
    fseek(ecb, 14, SEEK_CUR);
    for (int j = 0; j < 8; j++)
		fscanf(ecb, "%2hhx", &state[j]);

    fprintf(fp_ecb, "[ENCRYPT]\n\n");

    for (int i = 0; i < 400; i++) {
        fprintf(fp_ecb, "COUNT = %d\n", i);

        fprintf(fp_ecb, "KEY1 = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");

        fprintf(fp_ecb, "KEY2 = ");
        for (int j = 8; j < 16; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");

        fprintf(fp_ecb, "KEY3 = ");
        for (int j = 16; j < 24; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");

        TDES_CTX ctx;
        TDES_set_key(&ctx, (uint32_t *)tkey, 24);

        fprintf(fp_ecb, "PLAINTEXT = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", state[j]);
        fprintf(fp_ecb, "\n");

        fprintf(fp_ecb, "CIPHERTEXT = ");
        for (int j = 0; j < 10000; j++) {
            if (j == 9998)
                memcpy(tmp[0], state, 8);
            if (j == 9999)
                memcpy(tmp[1], state, 8);
            if (TDES_ECB_Enc(&ctx, (uint32_t *)state, (uint32_t *)state, 8) != 1) {
                fprintf(stderr, "TDEA MCT Fail(ENC)\n");
                return -1;
            }
        }
        int l1 = 0, l2 = 0;
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", state[j]);
        fprintf(fp_ecb, "\n\n");
        if ((memcmp(tkey, tkey + 8, 8) != 0 && memcmp(tkey, tkey + 16, 8) == 0) || (memcmp(tkey, tkey + 8, 8) != 0 && memcmp(tkey, tkey + 16, 8) != 0)) {
            l1 = 0;
        }else {
            l1 = 1;
        }

        if ((memcmp(tkey, tkey + 8, 8) == 0 && memcmp(tkey, tkey + 16, 8) == 0) || (memcmp(tkey, tkey + 8, 8) != 0 && memcmp(tkey, tkey + 16, 8) == 0)) {
            l2 = 0;
        }else {
            l2 = 1;
        }

        for (int j = 0; j < 8; j++) {
            tkey[j] ^= state[j];
        }
        if (l1 == 0) {
            for (int j = 0; j < 8; j++) {
                tkey[j + 8] ^= tmp[1][j];
            }
        }else {
            for (int j = 0; j < 8; j++) {
                tkey[j + 8] ^= state[j];
            }
        }

        if (l2 == 0) {
            for (int j = 0; j < 8; j++) {
                tkey[16 + j] ^= state[j];
            }
        }else {
            for (int j = 0; j < 8; j++) {
                tkey[16 + j] ^= tmp[0][j];
            }
        }
    }
    fseek(ecb, 0xEC41, SEEK_SET);
    fprintf(fp_ecb, "[DECRYPT]\n\n");

    for (int j = 0; j < 8; j++)
		fscanf(ecb, "%2hhx", &tkey[j]); // 2자리씩 hex값 읽어오기
    fseek(ecb, 9, SEEK_CUR);
    for (int j = 8; j < 16; j++)
		fscanf(ecb, "%2hhx", &tkey[j]);
    fseek(ecb, 9, SEEK_CUR);
    for (int j = 16; j < 24; j++)
		fscanf(ecb, "%2hhx", &tkey[j]);
    fseek(ecb, 14, SEEK_CUR);
    for (int j = 0; j < 8; j++)
		fscanf(ecb, "%2hhx", &state[j]);

    for (int i = 0; i < 400; i++) {
        fprintf(fp_ecb, "COUNT = %d\n", i);

        fprintf(fp_ecb, "KEY1 = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");

        fprintf(fp_ecb, "KEY2 = ");
        for (int j = 8; j < 16; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");

        fprintf(fp_ecb, "KEY3 = ");
        for (int j = 16; j < 24; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");

        TDES_CTX ctx;
        TDES_set_key(&ctx, (uint32_t *)tkey, 24);

        fprintf(fp_ecb, "CIPHERTEXT = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", state[j]);
        fprintf(fp_ecb, "\n");

        fprintf(fp_ecb, "PLAINTEXT = ");
        for (int j = 0; j < 10000; j++) {
            if (j == 9998)
                memcpy(tmp[0], state, 8);
            if (j == 9999)
                memcpy(tmp[1], state, 8);
            if (TDES_ECB_Dec(&ctx, (uint32_t *)state, (uint32_t *)state, 8) != 1) {
                fprintf(stderr, "TDEA MCT Fail(Dec)\n");
                return -1;
            }
        }

        int l1 = 0, l2 = 0;
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", state[j]);
        fprintf(fp_ecb, "\n\n");
        if ((memcmp(tkey, tkey + 8, 8) != 0 && memcmp(tkey, tkey + 16, 8) == 0) || (memcmp(tkey, tkey + 8, 8) != 0 && memcmp(tkey, tkey + 16, 8) != 0)) {
            l1 = 0;
        }else {
            l1 = 1;
        }

        if ((memcmp(tkey, tkey + 8, 8) == 0 && memcmp(tkey, tkey + 16, 8) == 0) || (memcmp(tkey, tkey + 8, 8) != 0 && memcmp(tkey, tkey + 16, 8) == 0)) {
            l2 = 0;
        }else {
            l2 = 1;
        }

        for (int j = 0; j < 8; j++) {
            tkey[j] ^= state[j];
        }
        if (l1 == 0) {
            for (int j = 0; j < 8; j++) {
                tkey[j + 8] ^= tmp[1][j];
            }
        }else {
            for (int j = 0; j < 8; j++) {
                tkey[j + 8] ^= state[j];
            }
        }

        if (l2 == 0) {
            for (int j = 0; j < 8; j++) {
                tkey[16 + j] ^= state[j];
            }
        }else {
            for (int j = 0; j < 8; j++) {
                tkey[16 + j] ^= tmp[0][j];
            }
        }
    }
}
