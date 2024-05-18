/**
 * @cbc_mct_test.c monte carlo test
 * null padding version
 * reference: https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-20.pdf
 * The Monte Carlo Test for the Encryption Process - TCBC Mode
 * Monte Carlo Test for the Decryption Process - TCBC Mode
*/

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "tdes.h"

int main() {
    FILE *fp_cbc = fopen("TDES_CBC_Monte.rsp", "w");

    FILE* cbc = fopen("TCBCMonte3.rsp", "r");

    unsigned char tkey[24];
    unsigned char plaintext[8];
    unsigned char ciphertext[2][8];
    unsigned char IV[8];

    if (fp_cbc == NULL || cbc == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }
    fprintf(fp_cbc,"# CAVS 11.1 for my TDEA\n");
    fprintf(fp_cbc,"# Config Info for : \"tdes_values\"\n");
    fprintf(fp_cbc,"# TDES Monte Carlo (Modes) Test for CBC\n");
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

    fseek(cbc, 0xb2, SEEK_SET);
    for (int j = 0; j < 8; j++)
		fscanf(cbc, "%2hhx", &tkey[j]); // 2자리씩 hex값 읽어오기
    fseek(cbc, 9, SEEK_CUR);
    for (int j = 8; j < 16; j++)
		fscanf(cbc, "%2hhx", &tkey[j]);
    fseek(cbc, 9, SEEK_CUR);
    for (int j = 16; j < 24; j++)
		fscanf(cbc, "%2hhx", &tkey[j]);
    fseek(cbc, 7, SEEK_CUR);
    for (int j = 0; j < 8; j++)
	    fscanf(cbc, "%2hhx", &IV[j]);
    fseek(cbc, 14, SEEK_CUR);
    for (int j = 0; j < 8; j++)
		fscanf(cbc, "%2hhx", &plaintext[j]);

    fprintf(fp_cbc, "[ENCRYPT]\n\n");

    for (int i = 0; i < 400; i++) {
        fprintf(fp_cbc, "COUNT = %d\n", i);

        fprintf(fp_cbc, "KEY1 = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_cbc, "%02x", tkey[j]);
        fprintf(fp_cbc, "\n");

        fprintf(fp_cbc, "KEY2 = ");
        for (int j = 8; j < 16; j++)
            fprintf(fp_cbc, "%02x", tkey[j]);
        fprintf(fp_cbc, "\n");

        fprintf(fp_cbc, "KEY3 = ");
        for (int j = 16; j < 24; j++)
            fprintf(fp_cbc, "%02x", tkey[j]);
        fprintf(fp_cbc, "\n");

        TDES_CTX ctx;
        TDES_set_key(&ctx, (uint32_t *)tkey, 24);

        fprintf(fp_cbc, "IV = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_cbc, "%02x", IV[j]);
        fprintf(fp_cbc, "\n");
        ctx.IV = (uint32_t *)IV;

        fprintf(fp_cbc, "PLAINTEXT = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_cbc, "%02x", plaintext[j]);
        fprintf(fp_cbc, "\n");

        fprintf(fp_cbc, "CIPHERTEXT = ");
        for (int j = 0; j < 10000; j++) {
            if (TDES_CBC_Enc(&ctx, (uint32_t *)plaintext, (uint32_t *)plaintext, 8) != 1) {
                fprintf(stderr, "TDEA MCT Fail(ENC)\n");
                return -1;
            }

            unsigned char tmp[8];
            if (j == 0)
                memcpy(tmp, IV, 8);
            if (j == 9999)
                continue;
            memcpy(IV, plaintext, 8);
            if (j % 2)
                memcpy(ciphertext[1], plaintext, 8);
            else
                memcpy(ciphertext[0], plaintext, 8);

            if (j == 0)
                memcpy(plaintext, tmp, 8);
            else
                memcpy(plaintext, ciphertext[(j - 1)%2], 8);

        }
        int l1 = 0, l2 = 0;
        for (int j = 0; j < 8; j++)
            fprintf(fp_cbc, "%02x", plaintext[j]);
        fprintf(fp_cbc, "\n\n");
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
            tkey[j] ^= plaintext[j];
        }
        if (l1 == 0) {
            for (int j = 0; j < 8; j++) {
                tkey[j + 8] ^= ciphertext[0][j];
            }
        }else {
            for (int j = 0; j < 8; j++) {
                tkey[j + 8] ^= plaintext[j];
            }
        }

        if (l2 == 0) {
            for (int j = 0; j < 8; j++) {
                tkey[16 + j] ^= plaintext[j];
            }
        }else {
            for (int j = 0; j < 8; j++) {
                tkey[16 + j] ^= ciphertext[1][j];
            }
        }
        memcpy(IV, plaintext, 8);
        memcpy(plaintext, ciphertext[0], 8);
    }
    fseek(cbc, 0x11031, SEEK_SET);
    fprintf(fp_cbc, "[DECRYPT]\n\n");

   for (int j = 0; j < 8; j++)
		fscanf(cbc, "%2hhx", &tkey[j]); // 2자리씩 hex값 읽어오기
    fseek(cbc, 9, SEEK_CUR);
    for (int j = 8; j < 16; j++)
		fscanf(cbc, "%2hhx", &tkey[j]);
    fseek(cbc, 9, SEEK_CUR);
    for (int j = 16; j < 24; j++)
		fscanf(cbc, "%2hhx", &tkey[j]);
    fseek(cbc, 7, SEEK_CUR);
    for (int j = 0; j < 8; j++)
	    fscanf(cbc, "%2hhx", &IV[j]);
    fseek(cbc, 14, SEEK_CUR);
    for (int j = 0; j < 8; j++)
		fscanf(cbc, "%2hhx", &plaintext[j]);

    for (int i = 0; i < 400; i++) {
        fprintf(fp_cbc, "COUNT = %d\n", i);

        fprintf(fp_cbc, "KEY1 = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_cbc, "%02x", tkey[j]);
        fprintf(fp_cbc, "\n");

        fprintf(fp_cbc, "KEY2 = ");
        for (int j = 8; j < 16; j++)
            fprintf(fp_cbc, "%02x", tkey[j]);
        fprintf(fp_cbc, "\n");

        fprintf(fp_cbc, "KEY3 = ");
        for (int j = 16; j < 24; j++)
            fprintf(fp_cbc, "%02x", tkey[j]);
        fprintf(fp_cbc, "\n");

        TDES_CTX ctx;
        TDES_set_key(&ctx, (uint32_t *)tkey, 24);

        fprintf(fp_cbc, "IV = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_cbc, "%02x", IV[j]);
        fprintf(fp_cbc, "\n");
        ctx.IV = (uint32_t *)IV;

        fprintf(fp_cbc, "CIPHERTEXT = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_cbc, "%02x", plaintext[j]);
        fprintf(fp_cbc, "\n");

        fprintf(fp_cbc, "PLAINTEXT = ");
        for (int j = 0; j < 10000; j++) {
            if (j == 9998)
                memcpy(ciphertext[0], plaintext, 8);
            if (j == 9999)
                memcpy(ciphertext[1], plaintext, 8);
            unsigned char tmp[8];
            memcpy(tmp, plaintext, 8);
            if (TDES_CBC_Dec(&ctx, (uint32_t *)plaintext, (uint32_t *)plaintext, 8) != 1) {
                fprintf(stderr, "TDEA MCT Fail(DEC)\n");
                return -1;
            }
            memcpy(IV, tmp, 8);
        }
        int l1 = 0, l2 = 0;
        for (int j = 0; j < 8; j++)
            fprintf(fp_cbc, "%02x", plaintext[j]);
        fprintf(fp_cbc, "\n\n");
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
            tkey[j] ^= plaintext[j];
        }
        if (l1 == 0) {
            for (int j = 0; j < 8; j++) {
                tkey[j + 8] ^= ciphertext[1][j];
            }
        }else {
            for (int j = 0; j < 8; j++) {
                tkey[j + 8] ^= plaintext[j];
            }
        }

        if (l2 == 0) {
            for (int j = 0; j < 8; j++) {
                tkey[16 + j] ^= plaintext[j];
            }
        }else {
            for (int j = 0; j < 8; j++) {
                tkey[16 + j] ^= ciphertext[0][j];
            }
        }

    }
}
