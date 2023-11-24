#include "aes.h"

// Galois filed 연산 in MixColumns
inline int8_t GF_xtime(int8_t gf) {
    return ((((gf >> 7) & 0x01) == 1) ? (gf << 1) ^ 0x1b : gf << 1);
}

// key확장시 g함수 구현 for AES128
const int8_t round_Constant[10] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };


void AES_AddRoundkey(int8_t state[16], int8_t roundkey[16]) {

    for (int i = 0; i < 16; i++) {
        state[i] ^= roundkey[i];
    }

}

void AES_SubBytes(int8_t state[16]) {

    for (int i = 0; i < 16; i++) {
        state[i] = Sbox[state[i]];
    }

}

void AES_InvSubBytes(int8_t state[16]) {

    for (int i = 0; i < 16; i++) {
        state[i] = ISbox[state[i]];
    }

}

void AES_ShiftRows(int8_t state[16]) {

    int8_t temp;

    temp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = temp;

    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;

    temp = state[3];
    state[3] = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = temp;
}

void AES_InvShiftRows(int8_t state[16]) {

    int8_t temp;

    temp = state[13];
    state[13] = state[9];
    state[9] = state[5];
    state[5] = state[1];
    state[1] = temp;

    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;

    temp = state[15];
    state[15] = state[3];
    state[3] = state[7];
    state[7] = state[11];
    state[11] = temp;

}

void AES_MixColumns(int8_t state[16]) {

    int8_t sum, tmp0, tmp1, tmp2, tmp3;

    sum = state[0] ^ state[1] ^ state[2] ^ state[3];
    tmp0 = GF_xtime(state[0] ^ state[1]);
    tmp1 = GF_xtime(state[1] ^ state[2]);
    tmp2 = GF_xtime(state[2] ^ state[3]);
    tmp3 = GF_xtime(state[3] ^ state[0]);

    state[0] ^= tmp0 ^ sum;
    state[1] ^= tmp1 ^ sum;
    state[2] ^= tmp2 ^ sum;
    state[3] ^= tmp3 ^ sum;

    sum = state[4] ^ state[5] ^ state[6] ^ state[7];
    tmp0 = GF_xtime(state[4] ^ state[5]);
    tmp1 = GF_xtime(state[5] ^ state[6]);
    tmp2 = GF_xtime(state[6] ^ state[7]);
    tmp3 = GF_xtime(state[7] ^ state[4]);

    state[4] ^= tmp0 ^ sum;
    state[5] ^= tmp1 ^ sum;
    state[6] ^= tmp2 ^ sum;
    state[7] ^= tmp3 ^ sum;

    sum = state[8] ^ state[9] ^ state[10] ^ state[11];
    tmp0 = GF_xtime(state[8] ^ state[9]);
    tmp1 = GF_xtime(state[9] ^ state[10]);
    tmp2 = GF_xtime(state[10] ^ state[11]);
    tmp3 = GF_xtime(state[11] ^ state[8]);

    state[8] ^= tmp0 ^ sum;
    state[9] ^= tmp1 ^ sum;
    state[10] ^= tmp2 ^ sum;
    state[11] ^= tmp3 ^ sum;

    sum = state[12] ^ state[13] ^ state[14] ^ state[15];
    tmp0 = GF_xtime(state[12] ^ state[13]);
    tmp1 = GF_xtime(state[13] ^ state[14]);
    tmp2 = GF_xtime(state[14] ^ state[15]);
    tmp3 = GF_xtime(state[15] ^ state[12]);

    state[12] ^= tmp0 ^ sum;
    state[13] ^= tmp1 ^ sum;
    state[14] ^= tmp2 ^ sum;
    state[15] ^= tmp3 ^ sum;
}

void AES_InvMixColumns(int8_t state[16]) {

    int8_t sum8, tmp0, tmp1, tmp2, tmp3;

    sum8 = GF_xtime(GF_xtime(GF_xtime(state[0] ^ state[1] ^ state[2] ^ state[3])));
    tmp0 = sum8 ^ GF_xtime(GF_xtime(state[0]) ^ state[0]) ^ GF_xtime(state[1]) ^ state[1]
        ^ GF_xtime(GF_xtime(state[2])) ^ state[2] ^ state[3];
    tmp1 = sum8 ^ state[0] ^ GF_xtime(GF_xtime(state[1]) ^ state[1])
        ^ GF_xtime(state[2]) ^ state[2] ^ GF_xtime(GF_xtime(state[3])) ^ state[3];
    tmp2 = sum8 ^ GF_xtime(GF_xtime(state[0])) ^ state[0] ^ state[1]
        ^ GF_xtime(GF_xtime(state[2]) ^ state[2]) ^ GF_xtime(state[3]) ^ state[3];
    tmp3 = sum8 ^ GF_xtime(state[0]) ^ state[0] ^ GF_xtime(GF_xtime(state[1])) ^ state[1]
        ^ state[2] ^ GF_xtime(GF_xtime(state[3]) ^ state[3]);
    state[0] = tmp0;
    state[1] = tmp1;
    state[2] = tmp2;
    state[3] = tmp3;

    sum8 = GF_xtime(GF_xtime(GF_xtime(state[4] ^ state[5] ^ state[6] ^ state[7])));

    tmp0 = sum8 ^ GF_xtime(GF_xtime(state[4]) ^ state[4]) ^ GF_xtime(state[5]) ^ state[5]
        ^ GF_xtime(GF_xtime(state[6])) ^ state[6] ^ state[7];
    tmp1 = sum8 ^ state[4] ^ GF_xtime(GF_xtime(state[5]) ^ state[5])
        ^ GF_xtime(state[6]) ^ state[6] ^ GF_xtime(GF_xtime(state[7])) ^ state[7];
    tmp2 = sum8 ^ GF_xtime(GF_xtime(state[4])) ^ state[4] ^ state[5]
        ^ GF_xtime(GF_xtime(state[6]) ^ state[6]) ^ GF_xtime(state[7]) ^ state[7];
    tmp3 = sum8 ^ GF_xtime(state[4]) ^ state[4] ^ GF_xtime(GF_xtime(state[5])) ^ state[5]
        ^ state[6] ^ GF_xtime(GF_xtime(state[7]) ^ state[7]);
    state[4] = tmp0;
    state[5] = tmp1;
    state[6] = tmp2;
    state[7] = tmp3;

    sum8 = GF_xtime(GF_xtime(GF_xtime(state[8] ^ state[9] ^ state[10] ^ state[11])));
    tmp0 = sum8 ^ GF_xtime(GF_xtime(state[8]) ^ state[8]) ^ GF_xtime(state[9]) ^ state[9]
        ^ GF_xtime(GF_xtime(state[10])) ^ state[10] ^ state[11];
    tmp1 = sum8 ^ state[8] ^ GF_xtime(GF_xtime(state[9]) ^ state[9])
        ^ GF_xtime(state[10]) ^ state[10] ^ GF_xtime(GF_xtime(state[11])) ^ state[11];
    tmp2 = sum8 ^ GF_xtime(GF_xtime(state[8])) ^ state[8] ^ state[9]
        ^ GF_xtime(GF_xtime(state[10]) ^ state[10]) ^ GF_xtime(state[11]) ^ state[11];
    tmp3 = sum8 ^ GF_xtime(state[8]) ^ state[8] ^ GF_xtime(GF_xtime(state[9])) ^ state[9]
        ^ state[10] ^ GF_xtime(GF_xtime(state[11]) ^ state[11]);
    state[8] = tmp0;
    state[9] = tmp1;
    state[10] = tmp2;
    state[11] = tmp3;

    sum8 = GF_xtime(GF_xtime(GF_xtime(state[12] ^ state[13] ^ state[14] ^ state[15])));
    tmp0 = sum8 ^ GF_xtime(GF_xtime(state[12]) ^ state[12]) ^ GF_xtime(state[13]) ^ state[13]
        ^ GF_xtime(GF_xtime(state[14])) ^ state[14] ^ state[15];
    tmp1 = sum8 ^ state[12] ^ GF_xtime(GF_xtime(state[13]) ^ state[13])
        ^ GF_xtime(state[14]) ^ state[14] ^ GF_xtime(GF_xtime(state[15])) ^ state[15];
    tmp2 = sum8 ^ GF_xtime(GF_xtime(state[12])) ^ state[12] ^ state[13]
        ^ GF_xtime(GF_xtime(state[14]) ^ state[14]) ^ GF_xtime(state[15]) ^ state[15];
    tmp3 = sum8 ^ GF_xtime(state[12]) ^ state[12] ^ GF_xtime(GF_xtime(state[13])) ^ state[13]
        ^ state[14] ^ GF_xtime(GF_xtime(state[15]) ^ state[15]);
    state[12] = tmp0;
    state[13] = tmp1;
    state[14] = tmp2;
    state[15] = tmp3;
}

void AES128_KeySchedule(int8_t round, int8_t key[])
{
    key[0] = key[0] ^ round_Constant[round - 1] ^ Sbox[key[13]];
	key[1] = key[1] ^ Sbox[key[14]];
	key[2] = key[2] ^ Sbox[key[15]];
	key[3] = key[3] ^ Sbox[key[12]];

	for (int i = 0; i < 12; i++)
		key[i + 4] = key[i] ^ key[i + 4];
}

void AES128_Encrypt(int8_t cipherText[16] ,int8_t plainText[16], int8_t key[16])
{
	int8_t state[16] = { 0, };

	for (int j = 0; j < 16; j++) {
        state[j] = plainText[j];
    }

	AES_AddRoundkey(state, key);

	//----------------------------------------------------------------------------
	for (int j = 1; j < 10; j++)// 1~9라운드
	{

		// Key expansion on-the-fly
		AES128_KeySchedule(j, key);

        AES_SubBytes(state);
		AES_ShiftRows(state);
		AES_MixColumns(state);
        AES_AddRoundkey(state, key);
		
	}

	//10 라운드
	AES128_KeySchedule(10, key);

	AES_SubBytes(state);
	AES_ShiftRows(state);
    AES_AddRoundkey(state, key);
	
}