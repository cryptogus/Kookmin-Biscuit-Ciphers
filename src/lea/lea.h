#pragma once

#include <stdint.h>

typedef unsigned char byte;
typedef uint32_t word;

//Little Endian
#define GETU32(pt) (((word)(pt)[3] << 24) ^ ((word)(pt)[2] << 16) ^ ((word)(pt)[1] <<  8) ^ ((word)(pt)[0]))
#define PUTU32(ct, st) { (ct)[3] = (byte)((st) >> 24); (ct)[2] = (byte)((st) >> 16); (ct)[1] = (byte)((st) >>  8); (ct)[0] = (byte)(st); }

void LEA128_ENC(word ciphertext[], word plaintext[], word Key[]);