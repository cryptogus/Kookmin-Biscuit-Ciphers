#ifndef __TDES_H
#define __TDES_H
#pragma once
#include <stdint.h>

typedef struct {
  uint8_t w[3][128]; /*  TDES round key      */
  uint32_t n;        /*  n-th block key, w[n]*/
  uint32_t *IV;      /* IV */
} TDES_CTX;


void keyexpansion(uint8_t *w, uint8_t *key);
void des_encrypt(uint32_t *in, uint32_t *out, uint32_t *w);
void des_decrypt(uint32_t *in, uint32_t *out, uint32_t *w);
#endif // __TDES_H