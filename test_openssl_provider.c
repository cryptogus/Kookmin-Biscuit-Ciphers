/**
* @ build error state
*/
// Explain about openssl provider
// https://youtu.be/mHrB1RKlkYk?si=zcnUdTsXImoAdhJr
// https://youtu.be/ztfbzsnHqKU?si=r5x80WTiU_ORoaWX
// https://docs.openssl.org/3.0/man7/provider-cipher/#description
// https://github.com/openssl/openssl/blob/master/README-PROVIDERS.md

// example of provider
// https://github.com/open-quantum-safe/oqs-provider

// build error
#include <stdint.h>
#include "LEA.h"

// SYNOPSIS 는 include 파일에 정의된 것들
#include <openssl/core_dispatch.h> // OSSL_CORE_MAKE_FUNC
//#include <openssl/core.h>
static OSSL_FUNC_cipher_newctx_fn lea_newctx;
static OSSL_FUNC_cipher_encrypt_init_fn lea_encrypt;

// https://docs.openssl.org/3.0/man3/OSSL_PROVIDER/#examples
OSSL_DISPATCH lea_functions[] = {
    { OSSL_FUNC_CIPHER_NEWCTX, (void (*)(void))lea_newctx },
    {.function_id = OSSL_FUNC_CIPHER_ENCRYPT_INIT, .function = lea_encrypt},
    { 0, NULL }
};

// LEA 관련 함수 구현
static void *lea_newctx(void *provctx) {
    // LEA 컨텍스트 생성 코드
}
#include <stdio.h>
static int lea_encrypt(void *cctx, const unsigned char *key,
                            size_t keylen, const unsigned char *iv,
                            size_t ivlen, const OSSL_PARAM params[]) {
    // LEA 암호화 초기화 코드
    word Plaintext[4] = { 0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c};
    word Key[4] = { 0x3c2d1e0f, 0x78695a4b, 0xb4a59687, 0xf0e1d2c3 };
    word Ciphertext[4] = { 0, }; // 354ec89f 18c6c628 a7c73255 fd8b6404
    if (keylen >= 16 && keylen <= 32)
        fprintf(stderr, "lea key len error\n");
    LEA128_ENC(Plaintext, (uint32_t)key, Ciphertext);


}
#include <openssl/provider.h>
// 모듈 초기화 함수
void register_lea(void) {
    // LEA 함수를 OpenSSL에 등록합니다.
    OSSL_PROVIDER *provider = OSSL_PROVIDER_load(NULL, "LEA");
    if (provider != NULL) {
        OSSL_PROVIDER_set_dispatch(provider, lea_functions);
    }

}
#include <openssl/evp.h>

int main(int argc, char *argv[]) {
    // OpenSSL 라이브러리 초기화
    OpenSSL_add_all_algorithms();

    // LEA 등록
    register_lea();

}
/**
 * Chat Gpt example
 */
/*
// LEA 컨텍스트 구조체
typedef struct {
    // LEA에 필요한 변수들
    unsigned char *key;
    size_t keylen;
} LEA_CTX;
#include <openssl/params.h>
// LEA 컨텍스트 생성
static void *lea_newctx(void *provctx) {
    LEA_CTX *ctx = OPENSSL_zalloc(sizeof(LEA_CTX));
    if (ctx == NULL) {
        return NULL; // 메모리 할당 실패
    }
    return ctx; // 생성한 컨텍스트 반환
}

// LEA 암호화 초기화
static int lea_encrypt_init(void *cctx, const unsigned char *key,
                            size_t keylen, const unsigned char *iv,
                            size_t ivlen, const OSSL_PARAM params[]) {
    LEA_CTX *ctx = (LEA_CTX *)cctx;

    // 키 및 길이 저장
    ctx->key = OPENSSL_memdup(key, keylen);
    if (ctx->key == NULL) {
        return 0; // 메모리 할당 실패
    }
    ctx->keylen = keylen;

    // IV 관련 설정을 원하면 여기에 추가
    return 1; // 성공적으로 초기화됨
}

static OSSL_DISPATCH lea_functions[] = {
    { OSSL_FUNC_CIPHER_NEWCTX, (void (*)(void))lea_newctx },
    { OSSL_FUNC_CIPHER_ENCRYPT_INIT, (void (*)(void))lea_encrypt_init },
    { 0, NULL } // 배열의 끝
};

void register_lea(void) {
    OSSL_PROVIDER *provider = OSSL_PROVIDER_load(NULL, "LEA");
    if (provider != NULL) {
        OSSL_PROVIDER_set_dispatch(provider, lea_functions);
    }
}

#include <openssl/evp.h>

void use_lea_algorithm() {
    // OpenSSL 라이브러리 초기화
    OpenSSL_add_all_algorithms();

    // LEA 등록
    register_lea();

    // 암호화에 사용할 컨텍스트와 키 정의
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    unsigned char key[16]; // LEA의 키 (16 바이트 예시)
    unsigned char iv[16];  // 초기화 벡터
    // 키와 IV 설정 ...

    // LEA 암호화 초기화
    if (EVP_EncryptInit_ex(ctx, EVP_CIPHER_fetch(NULL, "LEA-128", NULL), NULL, key, iv) != 1) {
        // 오류 처리
    }

    // 데이터 암호화 과정...

    // 사용 후 자원 정리
    EVP_CIPHER_CTX_free(ctx);
}
LEA 함수 구현: lea_newctx와 lea_encrypt_init을 포함한 LEA 알고리즘에 필요한 함수를 구현합니다.
OSSL_DISPATCH 배열 정의: lea_functions 배열에 LEA 관련 함수를 등록합니다.
프로바이더 등록: register_lea 함수를 작성하여 LEA를 OpenSSL 프로바이더에 등록합니다.
OpenSSL 사용: use_lea_algorithm 함수를 통해 LEA 암호화를 수행합니다.
*/
