#ifndef __TDES_H
#define __TDES_H
#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  uint8_t w[3][128]; /*  TDES round key      */
  uint32_t *IV;      /* IV */
} TDES_CTX;


void keyexpansion(uint8_t *w, uint8_t *key);
void des_encrypt(uint32_t *in, uint32_t *out, uint32_t *w);
void des_decrypt(uint32_t *in, uint32_t *out, uint32_t *w);

int TDES_set_key(TDES_CTX *ctx, const uint32_t *key,
                             size_t key_len);

int TDES_ECB_Enc(TDES_CTX *ctx, uint32_t *dest, uint32_t *src,
                             int32_t len);
int TDES_ECB_Dec(TDES_CTX *ctx, uint32_t *dest, uint32_t *src,
                             int32_t len);
int TDES_CBC_Enc(TDES_CTX *ctx, uint32_t *dest, uint32_t *src,
                             int32_t len);
int TDES_CBC_Dec(TDES_CTX *ctx, uint32_t *dest, uint32_t *src,
                             int32_t len);
int TDES_CTR(TDES_CTX *ctx, uint32_t *dest, uint32_t *src,
                         int32_t len);

#endif // __TDES_H