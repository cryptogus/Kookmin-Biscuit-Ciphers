#include "lea.h"

void byte2state(byte b[16], word state[4]) {
	state[0] = GETU32(b);
	state[1] = GETU32(b + 4);
	state[2] = GETU32(b + 8);
	state[3] = GETU32(b + 12);
}

void state2byte(word state[4], byte b[16]) {
	PUTU32(b, state[0]);
	PUTU32(b + 4, state[1]);
	PUTU32(b + 8, state[2]);
	PUTU32(b + 12, state[3]);
}

word ROL(word x, word i) {
	return ((x) << (i)) | ((x) >> (32 - (i)));
}
word ROR(word x, word i) {
	return ((x) >> (i)) | ((x) << (32 - (i)));
}
//#define ROL(x,i)  ((x)<<(i)) | ((x)>>(32 - (i)))
//#define ROR(x,i)  ((x)>>(i)) | ((x)<<(32 - (i)))

void round_func_enc(word X[]/*input*/, word rk[], word state[]/*output*/) {
	state[0] = ROL(((X[0] ^ rk[0]) + (X[1] ^ rk[1])) & 0xffffffff, 9);
	state[1] = ROR(((X[1] ^ rk[2]) + (X[2] ^ rk[3])) & 0xffffffff, 5);
	state[2] = ROR(((X[2] ^ rk[4]) + (X[3] ^ rk[5])) & 0xffffffff, 3);
	state[3] = X[0];
}

void round_func_dec(word X[]/*input*/, word rk[], word state[]/*output*/) {
	state[0] = X[3];
	state[1] = (ROR(X[0], 9) - (state[0] ^ rk[0])) ^ rk[1];
	state[2] = (ROL(X[1], 5) - (state[1] ^ rk[2])) ^ rk[3];
	state[3] = (ROL(X[2], 3) - (state[2] ^ rk[4])) ^ rk[5];
}

void LEA128_KeySchedule(word Key[], word rk[][6]) {
	
    word T[4] = { 0, };
	
	const word delta[8] = { 0xc3efe9db, 0x44626b02, 0x79e27c8a, 0x78df30ec, 0x715ea49e, 0xc785da0a, 0xe04ef22a,0xe5c40957 };
	
	T[0] = Key[0];
	T[1] = Key[1];
	T[2] = Key[2];
	T[3] = Key[3];

	for (int i = 0; i < 24; i++) {
		T[0] = ROL((T[0] + ROL(delta[i % 4], i)) & 0xffffffff, 1);
		T[1] = ROL((T[1] + ROL(delta[i % 4], i + 1))& 0xffffffff, 3);
		T[2] = ROL((T[2] + ROL(delta[i % 4], i + 2))& 0xffffffff, 6);
		T[3] = ROL((T[3] + ROL(delta[i % 4], i + 3))& 0xffffffff, 11);

		rk[i][0] = T[0];
		rk[i][1] = T[1];
		rk[i][2] = T[2];
		rk[i][3] = T[1];
		rk[i][4] = T[3];
		rk[i][5] = T[1];
		
	}
}

void LEA128_ENC(uint32_t ciphertext[], uint32_t plaintext[], uint32_t Key[]) {
	
	word rk[24][6] = { 0, };
	word state[4] = { 0, };
	word state2[4] = { 0, };

	LEA128_KeySchedule(Key, rk);
	
	round_func_enc(plaintext, rk[0], state);
	round_func_enc(state, rk[1], state2);
	round_func_enc(state2, rk[2], state);
	round_func_enc(state, rk[3], state2);
	round_func_enc(state2, rk[4], state);
	round_func_enc(state, rk[5], state2);
	round_func_enc(state2, rk[6], state);
	round_func_enc(state, rk[7], state2);
	round_func_enc(state2, rk[8], state);
	round_func_enc(state, rk[9], state2);
	round_func_enc(state2, rk[10], state);
	round_func_enc(state, rk[11], state2);
	round_func_enc(state2, rk[12], state);
	round_func_enc(state, rk[13], state2);
	round_func_enc(state2, rk[14], state);
	round_func_enc(state, rk[15], state2);
	round_func_enc(state2, rk[16], state);
	round_func_enc(state, rk[17], state2);
	round_func_enc(state2, rk[18], state);
	round_func_enc(state, rk[19], state2);
	round_func_enc(state2, rk[20], state);
	round_func_enc(state, rk[21], state2);
	round_func_enc(state2, rk[22], state);
	round_func_enc(state, rk[23], ciphertext);
}

void LEA128_DEC(uint32_t plaintext[], uint32_t ciphertext[], uint32_t Key[]) {
	
	word rk[24][6] = { 0, };
	word state[4] = { 0, };
	word state2[4] = { 0, };

	LEA128_KeySchedule(Key, rk);
	
	round_func_dec(ciphertext, rk[23], state);
	round_func_dec(state, rk[22], state2);
	round_func_dec(state2, rk[21], state);
	round_func_dec(state, rk[20], state2);
	round_func_dec(state2, rk[19], state);
	round_func_dec(state, rk[18], state2);
	round_func_dec(state2, rk[17], state);
	round_func_dec(state, rk[16], state2);
	round_func_dec(state2, rk[15], state);
	round_func_dec(state, rk[14], state2);
	round_func_dec(state2, rk[13], state);
	round_func_dec(state, rk[12], state2);
	round_func_dec(state2, rk[11], state);
	round_func_dec(state, rk[10], state2);
	round_func_dec(state2, rk[9], state);
	round_func_dec(state, rk[8], state2);
	round_func_dec(state2, rk[7], state);
	round_func_dec(state, rk[6], state2);
	round_func_dec(state2, rk[5], state);
	round_func_dec(state, rk[4], state2);
	round_func_dec(state2, rk[3], state);
	round_func_dec(state, rk[2], state2);
	round_func_dec(state2, rk[1], state);
	round_func_dec(state, rk[0], plaintext);
	
}

void LEA192_KeySchedule(word Key[], word rk[][6]) {
	
    word T[6] = { 0, };
	
	const word delta[8] = { 0xc3efe9db, 0x44626b02, 0x79e27c8a, 0x78df30ec, 0x715ea49e, 0xc785da0a, 0xe04ef22a,0xe5c40957 };
	
	T[0] = Key[0];
	T[1] = Key[1];
	T[2] = Key[2];
	T[3] = Key[3];
	T[4] = Key[4];
	T[5] = Key[5];

	for (int i = 0; i < 28; i++) {
		T[0] = ROL((T[0] + ROL(delta[i % 6], i)) & 0xffffffff, 1);
		T[1] = ROL((T[1] + ROL(delta[i % 6], i + 1))& 0xffffffff, 3);
		T[2] = ROL((T[2] + ROL(delta[i % 6], i + 2))& 0xffffffff, 6);
		T[3] = ROL((T[3] + ROL(delta[i % 6], i + 3))& 0xffffffff, 11);
		T[4] = ROL((T[4] + ROL(delta[i % 6], i + 4))& 0xffffffff, 13);
		T[5] = ROL((T[5] + ROL(delta[i % 6], i + 5))& 0xffffffff, 17);

		rk[i][0] = T[0];
		rk[i][1] = T[1];
		rk[i][2] = T[2];
		rk[i][3] = T[3];
		rk[i][4] = T[4];
		rk[i][5] = T[5];
		
	}
}

void LEA192_ENC(uint32_t ciphertext[], uint32_t plaintext[], uint32_t Key[]) {
	
	word rk[28][6] = { 0, };
	word state[4] = { 0, };
	word state2[4] = { 0, };

	LEA192_KeySchedule(Key, rk);
	
	round_func_enc(plaintext, rk[0], state);
	round_func_enc(state, rk[1], state2);
	round_func_enc(state2, rk[2], state);
	round_func_enc(state, rk[3], state2);
	round_func_enc(state2, rk[4], state);
	round_func_enc(state, rk[5], state2);
	round_func_enc(state2, rk[6], state);
	round_func_enc(state, rk[7], state2);
	round_func_enc(state2, rk[8], state);
	round_func_enc(state, rk[9], state2);
	round_func_enc(state2, rk[10], state);
	round_func_enc(state, rk[11], state2);
	round_func_enc(state2, rk[12], state);
	round_func_enc(state, rk[13], state2);
	round_func_enc(state2, rk[14], state);
	round_func_enc(state, rk[15], state2);
	round_func_enc(state2, rk[16], state);
	round_func_enc(state, rk[17], state2);
	round_func_enc(state2, rk[18], state);
	round_func_enc(state, rk[19], state2);
	round_func_enc(state2, rk[20], state);
	round_func_enc(state, rk[21], state2);
	round_func_enc(state2, rk[22], state);
	round_func_enc(state, rk[23], state2);
	round_func_enc(state2, rk[24], state);
	round_func_enc(state, rk[25], state2);	
	round_func_enc(state2, rk[26], state);
	round_func_enc(state, rk[27], ciphertext);
}

void LEA192_DEC(uint32_t plaintext[], uint32_t ciphertext[], uint32_t Key[]) {
	
	word rk[28][6] = { 0, };
	word state[4] = { 0, };
	word state2[4] = { 0, };

	LEA192_KeySchedule(Key, rk);

	round_func_dec(ciphertext, rk[27], state);
	round_func_dec(state, rk[26], state2);
	round_func_dec(state2, rk[25], state);
	round_func_dec(state, rk[24], state2);
	round_func_dec(state2, rk[23], state);
	round_func_dec(state, rk[22], state2);
	round_func_dec(state2, rk[21], state);
	round_func_dec(state, rk[20], state2);
	round_func_dec(state2, rk[19], state);
	round_func_dec(state, rk[18], state2);
	round_func_dec(state2, rk[17], state);
	round_func_dec(state, rk[16], state2);
	round_func_dec(state2, rk[15], state);
	round_func_dec(state, rk[14], state2);
	round_func_dec(state2, rk[13], state);
	round_func_dec(state, rk[12], state2);
	round_func_dec(state2, rk[11], state);
	round_func_dec(state, rk[10], state2);
	round_func_dec(state2, rk[9], state);
	round_func_dec(state, rk[8], state2);
	round_func_dec(state2, rk[7], state);
	round_func_dec(state, rk[6], state2);
	round_func_dec(state2, rk[5], state);
	round_func_dec(state, rk[4], state2);
	round_func_dec(state2, rk[3], state);
	round_func_dec(state, rk[2], state2);
	round_func_dec(state2, rk[1], state);
	round_func_dec(state, rk[0], plaintext);
}

void LEA256_KeySchedule(word Key[], word rk[][6]) {
	
    word T[8] = { 0, };
	
	const word delta[8] = { 0xc3efe9db, 0x44626b02, 0x79e27c8a, 0x78df30ec, 0x715ea49e, 0xc785da0a, 0xe04ef22a,0xe5c40957 };
	
	T[0] = Key[0];
	T[1] = Key[1];
	T[2] = Key[2];
	T[3] = Key[3];
	T[4] = Key[4];
	T[5] = Key[5];
	T[6] = Key[6];
	T[7] = Key[7];

	for (int i = 0; i < 32; i++) {
		T[((6*i) % 8)] = ROL((T[(6*i % 8)] + ROL(delta[i % 8], i)) & 0xffffffff, 1);
		T[((6*i + 1) % 8)] = ROL((T[((6*i + 1) % 8)] + ROL(delta[i % 8], i + 1))& 0xffffffff, 3);
		T[((6*i + 2) % 8)] = ROL((T[((6*i + 2) % 8)] + ROL(delta[i % 8], i + 2))& 0xffffffff, 6);
		T[((6*i + 3) % 8)] = ROL((T[((6*i + 3) % 8)] + ROL(delta[i % 8], i + 3))& 0xffffffff, 11);
		T[((6*i + 4) % 8)] = ROL((T[((6*i + 4) % 8)] + ROL(delta[i % 8], i + 4))& 0xffffffff, 13);
		T[((6*i + 5) % 8)] = ROL((T[((6*i + 5) % 8)] + ROL(delta[i % 8], i + 5))& 0xffffffff, 17);

		rk[i][0] = T[((6*i) % 8)];
		rk[i][1] = T[((6*i + 1) % 8)];
		rk[i][2] = T[((6*i + 2) % 8)];
		rk[i][3] = T[((6*i + 3) % 8)];
		rk[i][4] = T[((6*i + 4) % 8)];
		rk[i][5] = T[((6*i + 5) % 8)];
		
	}
}

void LEA256_ENC(uint32_t ciphertext[], uint32_t plaintext[], uint32_t Key[]) {
	
	word rk[32][6] = { 0, };
	word state[4] = { 0, };
	word state2[4] = { 0, };

	LEA256_KeySchedule(Key, rk);
	
	
	round_func_enc(plaintext, rk[0], state);
	round_func_enc(state, rk[1], state2);
	round_func_enc(state2, rk[2], state);
	round_func_enc(state, rk[3], state2);
	round_func_enc(state2, rk[4], state);
	round_func_enc(state, rk[5], state2);
	round_func_enc(state2, rk[6], state);
	round_func_enc(state, rk[7], state2);
	round_func_enc(state2, rk[8], state);
	round_func_enc(state, rk[9], state2);
	round_func_enc(state2, rk[10], state);
	round_func_enc(state, rk[11], state2);
	round_func_enc(state2, rk[12], state);
	round_func_enc(state, rk[13], state2);
	round_func_enc(state2, rk[14], state);
	round_func_enc(state, rk[15], state2);
	round_func_enc(state2, rk[16], state);
	round_func_enc(state, rk[17], state2);
	round_func_enc(state2, rk[18], state);
	round_func_enc(state, rk[19], state2);
	round_func_enc(state2, rk[20], state);
	round_func_enc(state, rk[21], state2);
	round_func_enc(state2, rk[22], state);
	round_func_enc(state, rk[23], state2);
	round_func_enc(state2, rk[24], state);
	round_func_enc(state, rk[25], state2);	
	round_func_enc(state2, rk[26], state);
	round_func_enc(state, rk[27], state2);	
	round_func_enc(state2, rk[28], state);
	round_func_enc(state, rk[29], state2);	
	round_func_enc(state2, rk[30], state);
	round_func_enc(state, rk[31], ciphertext);
}

void LEA256_DEC(uint32_t plaintext[], uint32_t ciphertext[], uint32_t Key[]) {
	
	word rk[32][6] = { 0, };
	word state[4] = { 0, };
	word state2[4] = { 0, };

	LEA256_KeySchedule(Key, rk);

	round_func_dec(ciphertext, rk[31], state);
	round_func_dec(state, rk[30], state2);
	round_func_dec(state2, rk[29], state);
	round_func_dec(state, rk[28], state2);
	round_func_dec(state2, rk[27], state);
	round_func_dec(state, rk[26], state2);
	round_func_dec(state2, rk[25], state);
	round_func_dec(state, rk[24], state2);
	round_func_dec(state2, rk[23], state);
	round_func_dec(state, rk[22], state2);
	round_func_dec(state2, rk[21], state);
	round_func_dec(state, rk[20], state2);
	round_func_dec(state2, rk[19], state);
	round_func_dec(state, rk[18], state2);
	round_func_dec(state2, rk[17], state);
	round_func_dec(state, rk[16], state2);
	round_func_dec(state2, rk[15], state);
	round_func_dec(state, rk[14], state2);
	round_func_dec(state2, rk[13], state);
	round_func_dec(state, rk[12], state2);
	round_func_dec(state2, rk[11], state);
	round_func_dec(state, rk[10], state2);
	round_func_dec(state2, rk[9], state);
	round_func_dec(state, rk[8], state2);
	round_func_dec(state2, rk[7], state);
	round_func_dec(state, rk[6], state2);
	round_func_dec(state2, rk[5], state);
	round_func_dec(state, rk[4], state2);
	round_func_dec(state2, rk[3], state);
	round_func_dec(state, rk[2], state2);
	round_func_dec(state2, rk[1], state);
	round_func_dec(state, rk[0], plaintext);
}