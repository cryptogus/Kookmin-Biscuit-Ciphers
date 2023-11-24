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


// key 확장: on-the-fly
void AES128_KeySchedule(int8_t round, int8_t key[])
{
    key[0] = key[0] ^ round_Constant[round - 1] ^ Sbox[key[13]];
	key[1] = key[1] ^ Sbox[key[14]];
	key[2] = key[2] ^ Sbox[key[15]];
	key[3] = key[3] ^ Sbox[key[12]];

	for (int i = 0; i < 12; i++)
		key[i + 4] = key[i] ^ key[i + 4];
}

//key 확장 ver2: Round Key를 미리 만들어둠
void AES128_KeySchedule_2(int8_t key[], int8_t roundKey[][16])
{
    
	for (int j = 0; j < 16; j++) //word 0~3
		roundKey[0][j] = key[j];

	roundKey[1][0] = roundKey[0][0] ^ round_Constant[0] ^ Sbox[roundKey[0][13]];//G func
	roundKey[1][1] = roundKey[0][1] ^ Sbox[roundKey[0][14]];
	roundKey[1][2] = roundKey[0][2] ^ Sbox[roundKey[0][15]];
	roundKey[1][3] = roundKey[0][3] ^ Sbox[roundKey[0][12]];

	for (int i = 0; i < 12; i++)//word4~7
		roundKey[1][i + 4] = roundKey[1][i] ^ roundKey[0][i + 4];

	roundKey[2][0] = roundKey[1][0] ^ round_Constant[1] ^ Sbox[roundKey[1][13]];
	roundKey[2][1] = roundKey[1][1] ^ Sbox[roundKey[1][14]];
	roundKey[2][2] = roundKey[1][2] ^ Sbox[roundKey[1][15]];
	roundKey[2][3] = roundKey[1][3] ^ Sbox[roundKey[1][12]];

	for (int i = 0; i < 12; i++)//word8~11
		roundKey[2][i + 4] = roundKey[2][i] ^ roundKey[1][i + 4];

	roundKey[3][0] = roundKey[2][0] ^ round_Constant[2] ^ Sbox[roundKey[2][13]];
	roundKey[3][1] = roundKey[2][1] ^ Sbox[roundKey[2][14]];
	roundKey[3][2] = roundKey[2][2] ^ Sbox[roundKey[2][15]];
	roundKey[3][3] = roundKey[2][3] ^ Sbox[roundKey[2][12]];

	for (int i = 0; i < 12; i++)//word 12~15
		roundKey[3][i + 4] = roundKey[3][i] ^ roundKey[2][i + 4];

	roundKey[4][0] = roundKey[3][0] ^ round_Constant[3] ^ Sbox[roundKey[3][13]];
	roundKey[4][1] = roundKey[3][1] ^ Sbox[roundKey[3][14]];
	roundKey[4][2] = roundKey[3][2] ^ Sbox[roundKey[3][15]];
	roundKey[4][3] = roundKey[3][3] ^ Sbox[roundKey[3][12]];

	for (int i = 0; i < 12; i++)//word 16~19
		roundKey[4][i + 4] = roundKey[4][i] ^ roundKey[3][i + 4];

	roundKey[5][0] = roundKey[4][0] ^ round_Constant[4] ^ Sbox[roundKey[4][13]];
	roundKey[5][1] = roundKey[4][1] ^ Sbox[roundKey[4][14]];
	roundKey[5][2] = roundKey[4][2] ^ Sbox[roundKey[4][15]];
	roundKey[5][3] = roundKey[4][3] ^ Sbox[roundKey[4][12]];

	for (int i = 0; i < 12; i++)//word 20~23
		roundKey[5][i + 4] = roundKey[5][i] ^ roundKey[4][i + 4];

	roundKey[6][0] = roundKey[5][0] ^ round_Constant[5] ^ Sbox[roundKey[5][13]];
	roundKey[6][1] = roundKey[5][1] ^ Sbox[roundKey[5][14]];
	roundKey[6][2] = roundKey[5][2] ^ Sbox[roundKey[5][15]];
	roundKey[6][3] = roundKey[5][3] ^ Sbox[roundKey[5][12]];

	for (int i = 0; i < 12; i++)//word 24~27
		roundKey[6][i + 4] = roundKey[6][i] ^ roundKey[5][i + 4];

	roundKey[7][0] = roundKey[6][0] ^ round_Constant[6] ^ Sbox[roundKey[6][13]];
	roundKey[7][1] = roundKey[6][1] ^ Sbox[roundKey[6][14]];
	roundKey[7][2] = roundKey[6][2] ^ Sbox[roundKey[6][15]];
	roundKey[7][3] = roundKey[6][3] ^ Sbox[roundKey[6][12]];

	for (int i = 0; i < 12; i++)//word 28~31
		roundKey[7][i + 4] = roundKey[7][i] ^ roundKey[6][i + 4];

	roundKey[8][0] = roundKey[7][0] ^ round_Constant[7] ^ Sbox[roundKey[7][13]];
	roundKey[8][1] = roundKey[7][1] ^ Sbox[roundKey[7][14]];
	roundKey[8][2] = roundKey[7][2] ^ Sbox[roundKey[7][15]];
	roundKey[8][3] = roundKey[7][3] ^ Sbox[roundKey[7][12]];

	for (int i = 0; i < 12; i++)//word 32~35
		roundKey[8][i + 4] = roundKey[8][i] ^ roundKey[7][i + 4];

	roundKey[9][0] = roundKey[8][0] ^ round_Constant[8] ^ Sbox[roundKey[8][13]];
	roundKey[9][1] = roundKey[8][1] ^ Sbox[roundKey[8][14]];
	roundKey[9][2] = roundKey[8][2] ^ Sbox[roundKey[8][15]];
	roundKey[9][3] = roundKey[8][3] ^ Sbox[roundKey[8][12]];

	for (int i = 0; i < 12; i++)//word 36~39
		roundKey[9][i + 4] = roundKey[9][i] ^ roundKey[8][i + 4];

	roundKey[10][0] = roundKey[9][0] ^ round_Constant[9] ^ Sbox[roundKey[9][13]];
	roundKey[10][1] = roundKey[9][1] ^ Sbox[roundKey[9][14]];
	roundKey[10][2] = roundKey[9][2] ^ Sbox[roundKey[9][15]];
	roundKey[10][3] = roundKey[9][3] ^ Sbox[roundKey[9][12]];

	for (int i = 0; i < 12; i++)//word 40~43
		roundKey[10][i + 4] = roundKey[10][i] ^ roundKey[9][i + 4];

}

void AES128_Encrypt(int8_t cipherText[16] ,int8_t plainText[16], int8_t key[16])
{
	int8_t state[16] = { 0, };
    
    // memcpy 사용 X
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

    for (int j = 0; j < 16; j++) {
        cipherText[j] = state[j];
    }
	
}

void AES128_Decrypt(int8_t cipherText[16] ,int8_t plainText[16], int8_t key[16])
{
	int8_t state[16] = { 0, };
    int8_t roundKey[11][16] = { 0, };

	for (int j = 0; j < 16; j++) {
        state[j] = cipherText[j];
    }
    
    AES128_KeySchedule_2(key, roundKey);
	AES_AddRoundkey(state, roundKey[0]);

	//----------------------------------------------------------------------------
	for (int j = 1; j < 10; j++)// 1~9라운드
	{

		// Key expansion on-the-fly
		AES128_KeySchedule(j, roundKey[j]);

        AES_SubBytes(state);
		AES_ShiftRows(state);
		AES_MixColumns(state);
        AES_AddRoundkey(state, roundKey[j]);
		
	}

	//10 라운드
	AES_SubBytes(state);
	AES_ShiftRows(state);
    AES_AddRoundkey(state, roundKey[10]);
	
}