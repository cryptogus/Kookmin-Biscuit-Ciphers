#include <stdio.h>
#include <stdint.h>

typedef unsigned char byte;
typedef uint32_t word;

//Little Endian
#define GETU32(pt) (((word)(pt)[3] << 24) ^ ((word)(pt)[2] << 16) ^ ((word)(pt)[1] <<  8) ^ ((word)(pt)[0]))
#define PUTU32(ct, st) { (ct)[3] = (byte)((st) >> 24); (ct)[2] = (byte)((st) >> 16); (ct)[1] = (byte)((st) >>  8); (ct)[0] = (byte)(st); }

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

void round_func(word X[]/*input*/, word rk[]/*�ش� ����Ű*/, word state[]/*output*/) {
	state[0] = ROL(((X[0] ^ rk[0]) + (X[1] ^ rk[1])) & 0xffffffff, 9);
	state[1] = ROR(((X[1] ^ rk[2]) + (X[2] ^ rk[3])) & 0xffffffff, 5);
	state[2] = ROR(((X[2] ^ rk[4]) + (X[3] ^ rk[5])) & 0xffffffff, 3);
	state[3] = X[0];
}

void LEA_Key_Schedule(word Key[], word rk[][6]) {
	
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

void LEA128_ENC(word ciphertext[], word plaintext[], word Key[]) {
	
	word rk[24][6] = { 0, };
	word state[4] = { 0, };
	word state2[4] = { 0, };

	LEA_Key_Schedule(Key, rk);
	
	
	round_func(plaintext, rk[0], state);
	round_func(state, rk[1], state2);
	round_func(state2, rk[2], state);
	round_func(state, rk[3], state2);
	round_func(state2, rk[4], state);
	round_func(state, rk[5], state2);
	round_func(state2, rk[6], state);
	round_func(state, rk[7], state2);
	round_func(state2, rk[8], state);
	round_func(state, rk[9], state2);
	round_func(state2, rk[10], state);
	round_func(state, rk[11], state2);
	round_func(state2, rk[12], state);
	round_func(state, rk[13], state2);
	round_func(state2, rk[14], state);
	round_func(state, rk[15], state2);
	round_func(state2, rk[16], state);
	round_func(state, rk[17], state2);
	round_func(state2, rk[18], state);
	round_func(state, rk[19], state2);
	round_func(state2, rk[20], state);
	round_func(state, rk[21], state2);
	round_func(state2, rk[22], state);
	round_func(state, rk[23], ciphertext);
	
}