#include "rsa2048.h"

// RSA 키 쌍 생성 함수
int generate_key_pair(RSA **private_key, RSA **public_key) {
    BIGNUM *e = BN_new();
    RSA *keypair = RSA_new();
    // 공개키는 일반적으로 65537을 고정적으로 사용하는 편이다. -> left to right binary method(left to right binary exponentiation) 알고리즘 사용에서와 개인키 생성에 있어서 유리하기 때문
    if (!BN_set_word(e, RSA_F4)) {
        return 0;
    }

    if (!RSA_generate_key_ex(keypair, KEY_LENGTH, e, NULL)) {
        return 0;
    }

    *private_key = keypair;
    *public_key = RSAPublicKey_dup(keypair);
    
    BN_free(e);
    return 1;
}

// RSA 암호화 함수
int rsa_encrypt(const char *plain_text, size_t plain_length, RSA *public_key, unsigned char *cipher_text) {
    int encrypted_length = RSA_public_encrypt(plain_length, (const unsigned char *)plain_text, cipher_text, public_key, RSA_PKCS1_PADDING);
    return encrypted_length;
}

// RSA 복호화 함수
int rsa_decrypt(const unsigned char *cipher_text, size_t cipher_length, RSA *private_key, char *decrypted_text) {
    int decrypted_length = RSA_private_decrypt(cipher_length, cipher_text, (unsigned char *)decrypted_text, private_key, RSA_PKCS1_PADDING);
    return decrypted_length;
}
