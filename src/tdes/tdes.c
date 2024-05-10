#include "tdes.h"

const uint8_t SBOX[512] = {
    14, 0,  4,  15, 13, 7,  1,  4,  2,  14, 15, 2,  11, 13, 8,  1,  3,  10, 10,
    6,  6,  12, 12, 11, 5,  9,  9,  5,  0,  3,  7,  8,  4,  15, 1,  12, 14, 8,
    8,  2,  13, 4,  6,  9,  2,  1,  11, 7,  15, 5,  12, 11, 9,  3,  7,  14, 3,
    10, 10, 0,  5,  6,  0,  13, 15, 3,  1,  13, 8,  4,  14, 7,  6,  15, 11, 2,
    3,  8,  4,  14, 9,  12, 7,  0,  2,  1,  13, 10, 12, 6,  0,  9,  5,  11, 10,
    5,  0,  13, 14, 8,  7,  10, 11, 1,  10, 3,  4,  15, 13, 4,  1,  2,  5,  11,
    8,  6,  12, 7,  6,  12, 9,  0,  3,  5,  2,  14, 15, 9,  10, 13, 0,  7,  9,
    0,  14, 9,  6,  3,  3,  4,  15, 6,  5,  10, 1,  2,  13, 8,  12, 5,  7,  14,
    11, 12, 4,  11, 2,  15, 8,  1,  13, 1,  6,  10, 4,  13, 9,  0,  8,  6,  15,
    9,  3,  8,  0,  7,  11, 4,  1,  15, 2,  14, 12, 3,  5,  11, 10, 5,  14, 2,
    7,  12, 7,  13, 13, 8,  14, 11, 3,  5,  0,  6,  6,  15, 9,  0,  10, 3,  1,
    4,  2,  7,  8,  2,  5,  12, 11, 1,  12, 10, 4,  14, 15, 9,  10, 3,  6,  15,
    9,  0,  0,  6,  12, 10, 11, 1,  7,  13, 13, 8,  15, 9,  1,  4,  3,  5,  14,
    11, 5,  12, 2,  7,  8,  2,  4,  14, 2,  14, 12, 11, 4,  2,  1,  12, 7,  4,
    10, 7,  11, 13, 6,  1,  8,  5,  5,  0,  3,  15, 15, 10, 13, 3,  0,  9,  14,
    8,  9,  6,  4,  11, 2,  8,  1,  12, 11, 7,  10, 1,  13, 14, 7,  2,  8,  13,
    15, 6,  9,  15, 12, 0,  5,  9,  6,  10, 3,  4,  0,  5,  14, 3,  12, 10, 1,
    15, 10, 4,  15, 2,  9,  7,  2,  12, 6,  9,  8,  5,  0,  6,  13, 1,  3,  13,
    4,  14, 14, 0,  7,  11, 5,  3,  11, 8,  9,  4,  14, 3,  15, 2,  5,  12, 2,
    9,  8,  5,  12, 15, 3,  10, 7,  11, 0,  14, 4,  1,  10, 7,  1,  6,  13, 0,
    11, 8,  6,  13, 4,  13, 11, 0,  2,  11, 14, 7,  15, 4,  0,  9,  8,  1,  13,
    10, 3,  14, 12, 3,  9,  5,  7,  12, 5,  2,  10, 15, 6,  8,  1,  6,  1,  6,
    4,  11, 11, 13, 13, 8,  12, 1,  3,  4,  7,  10, 14, 7,  10, 9,  15, 5,  6,
    0,  8,  15, 0,  14, 5,  2,  9,  3,  2,  12, 13, 1,  2,  15, 8,  13, 4,  8,
    6,  10, 15, 3,  11, 7,  1,  4,  10, 12, 9,  5,  3,  6,  14, 11, 5,  0,  0,
    14, 12, 9,  7,  2,  7,  2,  11, 1,  4,  14, 1,  7,  9,  4,  12, 10, 14, 8,
    2,  13, 0,  15, 6,  12, 10, 9,  13, 0,  15, 3,  3,  5,  5,  6,  8,  11};

const int8_t IPPERM[64] = {57, 49, 41, 33, 25, 17, 9,  1,  59, 51, 43, 35, 27,
                           19, 11, 3,  61, 53, 45, 37, 29, 21, 13, 5,  63, 55,
                           47, 39, 31, 23, 15, 7,  56, 48, 40, 32, 24, 16, 8,
                           0,  58, 50, 42, 34, 26, 18, 10, 2,  60, 52, 44, 36,
                           28, 20, 12, 4,  62, 54, 46, 38, 30, 22, 14, 6};

const int8_t IPINVPERM[64] = {
    39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9,  49, 17, 57, 25, 32, 0, 40, 8,  48, 16, 56, 24};

const int8_t PPERM[32] = {15, 6, 19, 20, 28, 11, 27, 16, 0,  14, 22,
                          25, 4, 17, 30, 9,  1,  7,  23, 13, 31, 26,
                          2,  8, 18, 12, 29, 5,  21, 10, 3,  24};

const int8_t PC1PERM[58] = {56, 48, 40, 32, 24, 16, 8,  0,  57, 49, 41, 33,
                            25, 17, 9,  1,  58, 50, 42, 34, 26, 18, 10, 2,
                            59, 51, 43, 35, -1, 62, 54, 46, 38, 30, 22, 14,
                            6,  61, 53, 45, 37, 29, 21, 13, 5,  60, 52, 44,
                            36, 28, 20, 12, 4,  27, 19, 11, 3,  -1};

const int8_t PC2PERM[48] = {13, 16, 10, 23, 0,  4,  2,  27, 14, 5,  20, 9,
                            22, 18, 11, 3,  25, 7,  15, 6,  26, 19, 12, 1,
                            44, 55, 34, 40, 50, 58, 33, 43, 54, 48, 36, 51,
                            47, 52, 42, 59, 37, 56, 49, 45, 53, 39, 32, 35};

void eperm(uint32_t *in, uint8_t *out) {
  out[0] = ((*in >> 19) & 0x20) | ((*in >> 3) & 0x1f);
  out[1] = ((*in & 0x1f) << 1) | ((*in >> 15) & 1);
  out[2] = ((*in & 0x1) << 5) | ((*in >> 11) & 0x1f);
  out[3] = ((*in >> 7) & 0x3e) | ((*in >> 23) & 1);
  out[4] = ((*in >> 3) & 0x20) | ((*in >> 19) & 0x1f);
  out[5] = ((*in >> 15) & 0x3e) | ((*in >> 31) & 1);
  out[6] = ((*in >> 11) & 0x20) | ((*in >> 27) & 0x1f);
  out[7] = ((*in >> 23) & 0x3e) | ((*in >> 7) & 1);
}

// multiple bit selection function
void perm(uint8_t *in, uint8_t *out, const int8_t *dperm, int32_t n) {
  uint32_t nb = n >> 3;
  uint32_t nc = 8;

  uint32_t i, j;
  uint32_t temp, sel;

  if (n == 48) {
    nb = 8;
    nc = 6;
  }

  for (i = 0; i < nb; i++) {
    temp = 0;
    for (j = 0; j < nc; j++) {
      sel = *dperm;
      dperm++;
      temp = temp << 1;
      temp |= (in[sel >> 3] >> ((sel & 7) ^ 7)) & 1;
    }
    out[i] = temp;
  }
}

static void fround(uint32_t *lr, uint32_t *ki) {
  uint32_t out;

  /*** f function start ***/
  uint32_t in_temp[2];

  /*** Expansion Permutation Box ***/
  eperm(&lr[1], (uint8_t *)in_temp);

  /*** XOR round key ***/
  in_temp[0] ^= ki[0];
  in_temp[1] ^= ki[1];
  uint8_t temp[8];

  /*** fsbox ***/
  uint8_t *data = (uint8_t *)in_temp;

  int i;
  for(i=0;i<8;i++)
    temp[i]=SBOX[(i<< 6) | data[i]];

  for(i=0;i<4;i++)
    data[i]=temp[i*2+1] | (temp[i*2] << 4);


  /*** Straight Permutation ***/
  perm((uint8_t *)in_temp, (uint8_t *)&out, PPERM, 32);

  /*** f function end ***/

  out ^= lr[0];
  lr[0] = lr[1];
  lr[1] = out;

}

// c et d are 28 bits each, bot encoded in 4 bytes
static void leftshift(uint8_t cd[8], int32_t n) {
  int32_t t;
  int32_t i;
  for (i = 0; i < 2; i++) {
    t = cd[0] >> (8 - n);
    cd[0] = (cd[0] << n) | (cd[1] >> (8 - n));
    cd[1] = (cd[1] << n) | (cd[2] >> (8 - n));
    cd[2] = (cd[2] << n) | (cd[3] >> (8 - n));
    cd[3] = (cd[3] << n) | (t << 4);
    cd += 4;
  }
}

void keyexpansion(uint8_t *w, uint8_t *key) {
  uint8_t cd[8];
  int sel = 0;
  uint32_t temp, i, j;
  const int8_t *pc1perm = PC1PERM;

  // gen_cd
  for (i = 0; i < 8; i++) {
    temp = 0;
    for (j = 0; j < 8; j++) {
      sel = *pc1perm;
      pc1perm++;
      if (sel == (-1)) {
        temp = temp << 4;
        break;
      }
      temp = temp << 1;
      temp |= (key[sel >> 3] >> ((sel & 7) ^ 7)) & 1;
    }
    cd[i] = temp;
  }

  for (i = 1; i <= 16; i++) {
    if ((i == 1) || (i == 2) || (i == 9) || (i == 16))
      leftshift(cd, 1);
    else
      leftshift(cd, 2);
    perm(cd, w, PC2PERM, 48);
    w += 8;
  }
}

// DES encryption with subkeys in w
void des_encrypt(uint32_t *in, uint32_t *out, uint32_t *w) {
  int32_t i;
  uint32_t temp[2];

  /*** Initial Permutation (IP) ***/
  perm((uint8_t *)in, (uint8_t *)temp, IPPERM, 64);

  for (i = 1; i <= 16; i++)
    fround(temp, w + ((i - 1) << 1));

  uint32_t tmp = temp[0];
  temp[0] = temp[1];
  temp[1] = tmp;

  /*** Final Permutation (INVERSE IP) ***/
  perm((uint8_t *)temp, (uint8_t *)out, IPINVPERM, 64);

}

// DES decryption with subkeys in w
void des_decrypt(uint32_t *in, uint32_t *out, uint32_t *w) {
  int32_t i;
  uint32_t temp[2];


  /*** Initial Permutation (IP) ***/
  perm((uint8_t *)in, (uint8_t *)temp, IPPERM, 64);

  for (i = 16; i >= 1; i--) {
    fround(temp, w + ((i - 1) << 1));
  }


  uint32_t tmp = temp[0];
  temp[0] = temp[1];
  temp[1] = tmp;

  perm((uint8_t *)temp, (uint8_t *)out, IPINVPERM, 64);

}

/**
 * @param key is master key. 16 or 24
 * @param key_len is master key's byte length
 *
 * @return 0 is fail, 1 is success
 */
int TDES_set_key(TDES_CTX *ctx, const uint32_t *key, size_t key_len) {


  if (key_len == 16) {  // 128-bit key
    keyexpansion(ctx->w[0], (uint8_t *)key);
    keyexpansion(ctx->w[1], (uint8_t *)(key + 2));
    memcpy(ctx->w[2], ctx->w[0], 128);

  } else if (key_len == 24) {  // 192-bit key
    keyexpansion(ctx->w[0], (uint8_t *)key);
    keyexpansion(ctx->w[1], (uint8_t *)(key + 2));
    keyexpansion(ctx->w[2], (uint8_t *)(key + 4));

  } else {
    return 0;
  }
  return 1;
}

/**
 * @return 0 is fail, 1 is success
 */
/**
 * @return 0 is fail, 1 is success
 */
int TDES_enc(TDES_CTX *ctx, uint32_t *dest, uint32_t *src) {
  if (ctx == NULL) {
    return 0;
  }

  des_encrypt(src, dest, (uint32_t *)ctx->w[0]);
  des_decrypt(dest, dest, (uint32_t *)ctx->w[1]);
  des_encrypt(dest, dest, (uint32_t *)ctx->w[2]);

  return 1;
}

int TDES_dec(TDES_CTX *ctx, uint32_t *dest, uint32_t *src) {
  if (ctx == NULL) {
    return 0;
  }

  des_decrypt(src, dest, (uint32_t *)ctx->w[2]);
  des_encrypt(dest, dest, (uint32_t *)ctx->w[1]);
  des_decrypt(dest, dest, (uint32_t *)ctx->w[0]);

  return 1;
}

int TDES_ECB_Enc(TDES_CTX *ctx, uint32_t *dest, uint32_t *src,
                             int32_t len) {
  for (; len > 0; src += 2, dest += 2, len -= 8) {
    if (TDES_enc(ctx, dest, src) != 1)
      return 0;
  }
  return 1;
}

int TDES_ECB_Dec(TDES_CTX *ctx, uint32_t *dest, uint32_t *src,
                             int32_t len) {
  for (; len > 0; src += 2, dest += 2, len -= 8) {
    if (TDES_dec(ctx, dest, src) != 1)
      return 0;
  }
  return 1;
}

int TDES_CBC_Enc(TDES_CTX *ctx, uint32_t *dest, uint32_t *src,
                             int32_t len) {
  uint32_t i;
  uint32_t temp[2];
  if (ctx->IV == NULL) {
    return 0;
  } else {
    memcpy(temp, ctx->IV, 8);
  }
  for (; len > 0; src += 2, dest += 2, len -= 8) {
    temp[0] ^= src[0];
    temp[1] ^= src[1];

    if (TDES_enc(ctx, dest, temp) != 1)
      return 0;
    memcpy(temp, dest, 8);
  }
  return 1;
}

int TDES_CBC_Dec(TDES_CTX *ctx, uint32_t *dest, uint32_t *src,
                             int32_t len) {
  uint32_t i;
  uint32_t temp[2];
  uint32_t P[2];
  if (ctx->IV == NULL) {
    return 0;
  } else {
    memcpy(temp, ctx->IV, 8);
  }

  for (; len > 0; src += 2, dest += 2, len -= 8) {
    if (TDES_dec(ctx, P, src) != 1)
      return 0;

    dest[0] = temp[0] ^ P[0];
    dest[1] = temp[1] ^ P[1];

    memcpy(temp, src, 8);
  }

  return 1;
}

static uint32_t little_to_big_endian(uint32_t val) {
  return ((val >> 24) & 0xFF) | ((val & 0xFF00) << 8) | ((val >> 8) & 0xFF00) |
         ((val & 0xFF) << 24);
}

int TDES_CTR(TDES_CTX *ctx, uint32_t *dest, uint32_t *src,
                         int32_t len) {
  uint32_t temp[2];
  uint32_t C[2];
  uint32_t i, ctr;

  if (ctx->IV != NULL) {
    temp[0] = ctx->IV[0];
  } else
    return 0;

  ctr = little_to_big_endian(ctx->IV[1]);

  for (; len > 0; src += 2, dest += 2, len -= 8) {
    temp[1] = little_to_big_endian(ctr++);
    if (TDES_enc(ctx, C, temp) != 1)
      return 0;
    dest[0] = src[0] ^ C[0];
    dest[1] = src[1] ^ C[1];
  }
  return 1;
}