#pragma once

#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/err.h>

// 키 길이 정의 (일반적으로 2048비트 사용)
#define KEY_LENGTH 2048

int generate_key_pair(RSA **private_key, RSA **public_key);
int rsa_encrypt(const char *plain_text, size_t plain_length, RSA *public_key, unsigned char *cipher_text);
int rsa_decrypt(const unsigned char *cipher_text, size_t cipher_length, RSA *private_key, char *decrypted_text);
void print_private_key(RSA *private_key);
void print_public_key(RSA *public_key);
int encrypt_message(const char *message, RSA *public_key, unsigned char **encrypted_message, size_t *encrypted_len);