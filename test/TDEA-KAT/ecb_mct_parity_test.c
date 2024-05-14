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
    FILE *fp_ecb = fopen("TDES_ECB_Monte.nif", "w");

    FILE* ecb = fopen("TECBMonte3.rsp", "r");

    unsigned char tkey[24];
    unsigned char plaintext[8];
    unsigned char ciphertext[2][8];


    const crypto_key_config_t config_tmp = {.version = kCryptoLibVersion1, .key_mode = kKeyModeDesEcb,
                                          .key_length = 24, .hw_backed = kHardenedBoolFalse, .exportable = kHardenedBoolTrue,
                                          .security_level = kSecurityLevelLow};
    crypto_blinded_key_t key = {.config = config_tmp, .keyblob_length = 24, .keyblob = (uint32_t *)tkey, .checksum = 0};

    crypto_word32_buf_t iv = {.data = (uint32_t *)NULL, .len = 0};

    if (fp_ecb == NULL || ecb == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }
    fprintf(fp_ecb,"# CAVS 11.1 for NIF\n");
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

        fprintf(fp_ecb, "PLAINTEXT = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", plaintext[j]);
        fprintf(fp_ecb, "\n");
        crypto_byte_buf_t cipher_output_n = {.data = plaintext, .len = 8};

        fprintf(fp_ecb, "CIPHERTEXT = ");
        for (int j = 0; j < 10000; j++) {
            crypto_const_byte_buf_t cipher_input_n = {.data = plaintext, .len = 8}; // 매 루프마다 스택영역에 변수 선언 (const라 값 변경이 불가능)
            if (j == 9998)
                memcpy(ciphertext[0], plaintext, 8);
            if (j == 9999)
                memcpy(ciphertext[1], plaintext, 8);
            if (nifcrypto_des(&key, iv, kBlockCipherModeEcb, kDesOperationEncrypt, cipher_input_n, kDesPaddingNull, &cipher_output_n) != kCryptoStatusOK) {
                fprintf(stderr, "nif des fail\n");
                return -1;
            }
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

        fprintf(fp_ecb, "CIPHERTEXT = ");
        for (int j = 0; j < 8; j++)
            fprintf(fp_ecb, "%02x", plaintext[j]);
        fprintf(fp_ecb, "\n");
        crypto_byte_buf_t cipher_output_n = {.data = plaintext, .len = 8};

        fprintf(fp_ecb, "PLAINTEXT = ");
        for (int j = 0; j < 10000; j++) {
            crypto_const_byte_buf_t cipher_input_n = {.data = plaintext, .len = 8}; // 매 루프마다 스택영역에 변수 선언 (const라 값 변경이 불가능)
            if (j == 9998)
                memcpy(ciphertext[0], plaintext, 8);
            if (j == 9999)
                memcpy(ciphertext[1], plaintext, 8);
            if (nifcrypto_des(&key, iv, kBlockCipherModeEcb, kDesOperationDecrypt, cipher_input_n, kDesPaddingNull, &cipher_output_n) != kCryptoStatusOK) {
                fprintf(stderr, "nif des fail\n");
                return -1;
            }
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
