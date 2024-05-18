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

void add_parity_bits(uint8_t *key) {
    int i, j, parity;

    // 각 바이트에 대해 패리티 비트 계산
    for (i = 0; i <= 7; i++) {
        parity = 0;

        // 해당 바이트에 있는 1의 개수를 세기
        for (j = 1; j < 8; j++) {
            if ((key[i] >> j) & 0x01) {
                parity ^= 0x01; // XOR 연산을 통해 1의 개수 세기
            }
        }

        // 패리티 비트 설정
        if (parity == 0) {
            key[i] |= 0x01; // 짝수 개의 1이면 패리티 비트를 1로 설정
        } else {
            key[i] &= ~0x01; // 홀수 개의 1이면 패리티 비트를 0으로 설정
        }
    }
}

int main() {
    FILE *fp_ecb = fopen("TDES_CBC_Monte.rsp", "w");

    FILE* ecb = fopen("TCBCMonte3.rsp", "r");

    unsigned char tkey[24];
    unsigned char plaintext[8];
    unsigned char ciphertext[2][8];
    unsigned char IV[8];

    if (fp_ecb == NULL || ecb == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }
    fprintf(fp_ecb,"# CAVS 11.1 for my TDEA\n");
    fprintf(fp_ecb,"# Config Info for : \"tdes_values\"\n");
    fprintf(fp_ecb,"# TDES Monte Carlo (Modes) Test for CBC\n");
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
    fseek(ecb, 7, SEEK_CUR);
    for (int j = 0; j < 8; j++)
	    fscanf(ecb, "%2hhx", &IV[j]);
    fseek(ecb, 14, SEEK_CUR);
    for (int j = 0; j < 8; j++)
		fscanf(ecb, "%2hhx", &plaintext[j]);

    fprintf(fp_ecb, "[ENCRYPT]\n\n");

    for (int i = 0; i < 400; i++) {
        fprintf(fp_ecb, "COUNT = %d\n", i);
        add_parity_bits(tkey);
        fprintf(fp_ecb, "KEY1 = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");
        add_parity_bits(tkey + 8);
        fprintf(fp_ecb, "KEY2 = ");
        for (int j = 8; j < 16; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");
        add_parity_bits(tkey + 16);
        fprintf(fp_ecb, "KEY3 = ");
        for (int j = 16; j < 24; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");
        TDES_CTX ctx;
        TDES_set_key(&ctx, (uint32_t *)tkey, 24);

        fprintf(fp_ecb, "IV = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", IV[j]);
        fprintf(fp_ecb, "\n");
        ctx.IV = (uint32_t *)IV;

        fprintf(fp_ecb, "PLAINTEXT = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", plaintext[j]);
        fprintf(fp_ecb, "\n");

        fprintf(fp_ecb, "CIPHERTEXT = ");
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
            fprintf(fp_ecb, "%02x", plaintext[j]);
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
    fseek(ecb, 0x11031, SEEK_SET);
    fprintf(fp_ecb, "[DECRYPT]\n\n");

   for (int j = 0; j < 8; j++)
		fscanf(ecb, "%2hhx", &tkey[j]); // 2자리씩 hex값 읽어오기
    fseek(ecb, 9, SEEK_CUR);
    for (int j = 8; j < 16; j++)
		fscanf(ecb, "%2hhx", &tkey[j]);
    fseek(ecb, 9, SEEK_CUR);
    for (int j = 16; j < 24; j++)
		fscanf(ecb, "%2hhx", &tkey[j]);
    fseek(ecb, 7, SEEK_CUR);
    for (int j = 0; j < 8; j++)
	    fscanf(ecb, "%2hhx", &IV[j]);
    fseek(ecb, 14, SEEK_CUR);
    for (int j = 0; j < 8; j++)
		fscanf(ecb, "%2hhx", &plaintext[j]);

    for (int i = 0; i < 400; i++) {
        fprintf(fp_ecb, "COUNT = %d\n", i);

        add_parity_bits(tkey);
        fprintf(fp_ecb, "KEY1 = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");
        add_parity_bits(tkey + 8);
        fprintf(fp_ecb, "KEY2 = ");
        for (int j = 8; j < 16; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");
        add_parity_bits(tkey + 16);
        fprintf(fp_ecb, "KEY3 = ");
        for (int j = 16; j < 24; j++)
            fprintf(fp_ecb, "%02x", tkey[j]);
        fprintf(fp_ecb, "\n");
        TDES_CTX ctx;
        TDES_set_key(&ctx, (uint32_t *)tkey, 24);

        fprintf(fp_ecb, "IV = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", IV[j]);
        fprintf(fp_ecb, "\n");
        ctx.IV = (uint32_t *)IV;

        fprintf(fp_ecb, "CIPHERTEXT = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", plaintext[j]);
        fprintf(fp_ecb, "\n");

        fprintf(fp_ecb, "PLAINTEXT = ");
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
            fprintf(fp_ecb, "%02x", plaintext[j]);
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
