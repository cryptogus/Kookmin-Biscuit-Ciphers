/*******************************************************************
 * 
 * 
 *  reference: https://github.com/PIPO-Blockcipher/PIPO-Blockcipher
 * 
 * 
 * 
 * 
 *******************************************************************/

#include "pipo.h"

// u32 MASTER_KEY[MASTER_KEY_SIZE * INT_NUM] = { 0, };
// u32 ROUND_KEY[(ROUND + 1) * INT_NUM] = { 0, };

void keyadd(u8* val, u8* rk)
{
	val[0] ^= rk[0];
	val[1] ^= rk[1];
	val[2] ^= rk[2];
	val[3] ^= rk[3];
	val[4] ^= rk[4];
	val[5] ^= rk[5];
	val[6] ^= rk[6];
	val[7] ^= rk[7];
}

void sbox(u8 *X)
{
	u8 T[3] = { 0, };
	//(MSB: x[7], LSB: x[0]) 
	// Input: x[7], x[6], x[5], x[4], x[3], x[2], x[1], x[0] 
	//S5_1
	X[5] ^= (X[7] & X[6]);
	X[4] ^= (X[3] & X[5]);
	X[7] ^= X[4];
	X[6] ^= X[3];
	X[3] ^= (X[4] | X[5]);
	X[5] ^= X[7];
	X[4] ^= (X[5] & X[6]);
	//S3
	X[2] ^= X[1] & X[0];
	X[0] ^= X[2] | X[1];
	X[1] ^= X[2] | X[0];
	X[2] = ~X[2];
	// Extend XOR
	X[7] ^= X[1];	X[3] ^= X[2];	X[4] ^= X[0];
	//S5_2
	T[0] = X[7];	T[1] = X[3];	T[2] = X[4];
	X[6] ^= (T[0] & X[5]);
	T[0] ^= X[6];
	X[6] ^= (T[2] | T[1]);
	T[1] ^= X[5];
	X[5] ^= (X[6] | T[2]);
	T[2] ^= (T[1] & T[0]);
	// Truncate XOR and bit change
	X[2] ^= T[0];	T[0] = X[1] ^ T[2];	X[1] = X[0]^T[1];	X[0] = X[7];	X[7] = T[0];
	T[1] = X[3];	X[3] = X[6];	X[6] = T[1];
	T[2] = X[4];	X[4] = X[5];	X[5] = T[2];
	// Output: (MSb) x[7], x[6], x[5], x[4], x[3], x[2], x[1], x[0] (LSb)
}

void inv_sbox(u8 *X)
{	//(MSB: x[7], LSB: x[0]) 
	// Input: x[7], x[6], x[5], x[4], x[3], x[2], x[1], x[0] 

	u8 T[3] = { 0, };

	T[0] = X[7]; X[7] = X[0]; X[0] = X[1]; X[1] = T[0];
	T[0] = X[7];	T[1] = X[6];	T[2] = X[5];
	// S52 inv
	X[4] ^= (X[3] | T[2]);
	X[3] ^= (T[2] | T[1]);
	T[1] ^= X[4];
	T[0] ^= X[3];
	T[2] ^= (T[1] & T[0]);
	X[3] ^= (X[4] & X[7]);
	//  Extended XOR
	X[0] ^= T[1]; X[1] ^= T[2]; X[2] ^= T[0];	
	T[0] = X[3]; X[3] = X[6]; X[6] = T[0];
	T[0] = X[5]; X[5] = X[4]; X[4] = T[0];
	//  Truncated XOR
	X[7] ^= X[1];	X[3] ^= X[2];	X[4] ^= X[0];
	// Inv_S5_1
	X[4] ^= (X[5] & X[6]);
	X[5] ^= X[7];
	X[3] ^= (X[4] | X[5]);
	X[6] ^= X[3];
	X[7] ^= X[4];
	X[4] ^= (X[3] & X[5]);
	X[5] ^= (X[7] & X[6]);
	// Inv_S3
	X[2] = ~X[2];
	X[1] ^= X[2] | X[0];
	X[0] ^= X[2] | X[1];
	X[2] ^= X[1] & X[0];
	 // Output: x[7], x[6], x[5], x[4], x[3], x[2], x[1], x[0]
}

//left rotation: (0,7,4,3,6,5,1,2)
void pbox(u8* X)
{
	X[1] = ((X[1] << 7)) | ((X[1] >> 1));
	X[2] = ((X[2] << 4)) | ((X[2] >> 4));
	X[3] = ((X[3] << 3)) | ((X[3] >> 5));
	X[4] = ((X[4] << 6)) | ((X[4] >> 2));
	X[5] = ((X[5] << 5)) | ((X[5] >> 3));
	X[6] = ((X[6] << 1)) | ((X[6] >> 7));
	X[7] = ((X[7] << 2)) | ((X[7] >> 6));

}

//left rotation(inverse): (0,1,4,5,2,3,7,6)
void inv_pbox(u8* X)
{
	X[1] = ((X[1] << 1)) | ((X[1] >> 7));
	X[2] = ((X[2] << 4)) | ((X[2] >> 4));
	X[3] = ((X[3] << 5)) | ((X[3] >> 3));
	X[4] = ((X[4] << 2)) | ((X[4] >> 6));
	X[5] = ((X[5] << 3)) | ((X[5] >> 5));
	X[6] = ((X[6] << 7)) | ((X[6] >> 1));
	X[7] = ((X[7] << 6)) | ((X[7] >> 2));
}
// pipo.h에서 MASTER_KEY_SIZE 참고
void ROUND_KEY_GEN(int ROUND, u32* MASTER_KEY, int MASTER_KEY_SIZE, u32* ROUND_KEY) {
	u32 i, j;
	u32 RCON = 0;
	
	for (i = 0; i < ROUND + 1; i++) {
		for (j = 0; j < INT_NUM; j++) 
			ROUND_KEY[INT_NUM*i + j] = MASTER_KEY[(INT_NUM*i + j) % (MASTER_KEY_SIZE*INT_NUM)];		
		ROUND_KEY[INT_NUM*i] ^= RCON;
		RCON++;
	}
	
}

void PIPO128_ENC(u32* CIPHER_TEXT, u32* PLAIN_TEXT, u32* MASTER_KEY) {
	int i = 0;
	u8* P = (u8*)PLAIN_TEXT;

    u32 ROUND_KEY[28] = {0,};
    ROUND_KEY_GEN(13, MASTER_KEY, 2, ROUND_KEY);
	u8* RK = (u8*)ROUND_KEY;
	
	keyadd(P, RK);

	for (i = 1; i < 14; i++)
	{
		//printf("\n  S Before : %02X %02X %02X %02X, %02X %02X %02X %02X", P[7], P[6], P[5], P[4], P[3], P[2], P[1], P[0]);
		sbox(P);
		//printf("\n  S After : %02X %02X %02X %02X, %02X %02X %02X %02X", P[7], P[6], P[5], P[4], P[3], P[2], P[1], P[0]);
		pbox(P);
		//printf("\n  R After : %02X %02X %02X %02X, %02X %02X %02X %02X", P[7], P[6], P[5], P[4], P[3], P[2], P[1], P[0]);
		keyadd(P, RK + (i * 8));
		//printf("\n  K Add: %02X %02X %02X %02X, %02X %02X %02X %02X", P[7], P[6], P[5], P[4], P[3], P[2], P[1], P[0]);
		
	}
	
    memcpy(CIPHER_TEXT, (u32 *)P, 8);
}

void PIPO256_ENC(u32* CIPHER_TEXT, u32* PLAIN_TEXT, u32* MASTER_KEY) {
	int i = 0;
	u8* P = (u8*)PLAIN_TEXT;

    u32 ROUND_KEY[36] = {0,};
    ROUND_KEY_GEN(17, MASTER_KEY, 4, ROUND_KEY);
	u8* RK = (u8*)ROUND_KEY;

	keyadd(P, RK);

	for (i = 1; i < 18; i++)
	{
		//printf("\n  S Before : %02X %02X %02X %02X, %02X %02X %02X %02X", P[7], P[6], P[5], P[4], P[3], P[2], P[1], P[0]);
		sbox(P);
		//printf("\n  S After : %02X %02X %02X %02X, %02X %02X %02X %02X", P[7], P[6], P[5], P[4], P[3], P[2], P[1], P[0]);
		pbox(P);
		//printf("\n  R After : %02X %02X %02X %02X, %02X %02X %02X %02X", P[7], P[6], P[5], P[4], P[3], P[2], P[1], P[0]);
		keyadd(P, RK + (i * 8));
		//printf("\n  K Add: %02X %02X %02X %02X, %02X %02X %02X %02X", P[7], P[6], P[5], P[4], P[3], P[2], P[1], P[0]);

	}

    memcpy(CIPHER_TEXT, (u32 *)P, 8);
}

void PIPO128_DEC(u32* PLAIN_TEXT, u32* CIPHER_TEXT, u32* MASTER_KEY) {
	int i = 0;
	u8* C = (u8*)CIPHER_TEXT;

	u32 ROUND_KEY[28] = {0,};
    ROUND_KEY_GEN(13, MASTER_KEY, 2, ROUND_KEY);
	u8* RK = (u8*)ROUND_KEY;
	
	for (i = 13; i > 0; i--)
	{
		keyadd(C, RK + (i * 8));
		inv_pbox(C);
		inv_sbox(C);
	}
	keyadd(C, RK);

    memcpy(PLAIN_TEXT, (u32 *)C, 8);
}

void PIPO256_DEC(u32* PLAIN_TEXT, u32* CIPHER_TEXT, u32* MASTER_KEY) {
	int i = 0;
	u8* C = (u8*)CIPHER_TEXT;

	u32 ROUND_KEY[36] = {0,};
    ROUND_KEY_GEN(17, MASTER_KEY, 4, ROUND_KEY);
	u8* RK = (u8*)ROUND_KEY;
	
	for (i = 17; i > 0; i--)
	{
		keyadd(C, RK + (i * 8));
		inv_pbox(C);
		inv_sbox(C);
	}
	keyadd(C, RK);
	
    memcpy(PLAIN_TEXT, (u32 *)C, 8);
}
