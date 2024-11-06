// SPDX-License-Identifier: Apache-2.0 AND MIT

/*
 * OpenSSL 3 provider
 *
 * @ref https://docs.openssl.org/3.0/man3/OSSL_DISPATCH/
 * @ref https://docs.openssl.org/3.0/man7/provider/
 * @ref https://docs.openssl.org/3.0/man7/provider-cipher/
 * @ref https://github.com/open-quantum-safe/oqs-provider/blob/main/oqsprov
 */

#include <errno.h>
#include <openssl/core.h>
#include <openssl/core_dispatch.h>
#include <openssl/core_names.h>
//#include <openssl/err.h>
//#include <openssl/objects.h>
//#include <openssl/params.h>
//#include <openssl/provider.h>
//#include <stdio.h>
//#include <string.h>
#include "aes.h"

#include <openssl/evp.h>
// aes example
static OSSL_FUNC_cipher_newctx_fn kbc_newctx;
static OSSL_FUNC_cipher_encrypt_init_fn kbc_aes128_enc_init;
static OSSL_FUNC_cipher_decrypt_init_fn kbc_aes128_dec_init;
static OSSL_FUNC_cipher_update_fn kbc_aes128_update;
static OSSL_FUNC_cipher_final_fn kbc_aes128_final;
static OSSL_FUNC_cipher_cipher_fn kbc_aes128_cipher;
static OSSL_FUNC_cipher_freectx_fn kbc_aes128_freectx;

OSSL_DISPATCH openkbc[] = {
    {OSSL_FUNC_CIPHER_NEWCTX, (void (*)(void))kbc_newctx},
    {OSSL_FUNC_CIPHER_ENCRYPT_INIT, (void (*)(void))kbc_aes128_enc_init},
    {OSSL_FUNC_CIPHER_DECRYPT_INIT, (void (*)(void))kbc_aes128_dec_init},
    {OSSL_FUNC_CIPHER_UPDATE, (void (*)(void))kbc_aes128_update},
    {OSSL_FUNC_CIPHER_FINAL, (void (*)(void))kbc_aes128_final},
    {OSSL_FUNC_CIPHER_CIPHER, (void (*)(void))kbc_aes128_cipher},
    {OSSL_FUNC_CIPHER_FREECTX, (void (*)(void))kbc_aes128_freectx},
};
/**
 * @brief provctx 포인터를 받아서 그 값이 NULL이 아닌 경우 해당 포인터가 가리키는 PROV_KBC_CTX 구조체의 libctx 필드를 반환
 */
#define PROV_KBC_LIBCTX_OF(provctx)                                            \
    provctx ? (((PROV_KBC_CTX *)provctx)->libctx) : NULL

typedef struct {
    OSSL_LIB_CTX *libctx;
    uint8_t *key;
} PROV_KBC_CTX;

static void *kbc_newctx(void *provctx) {
    PROV_KBC_CTX *kbcctx = OPENSSL_zalloc(sizeof(PROV_KBC_CTX));

    if (kbcctx == NULL)
        return NULL;
    kbcctx->libctx = PROV_KBC_LIBCTX_OF(provctx);

    return kbcctx;
}


static int kbc_encdec_init(void *vpkemctx, void *vkem,
                                     int operation) {
    PROV_KBC_CTX *pkemctx = (PROV_KBC_CTX *)vpkemctx;

    if (pkemctx == NULL || vkem == NULL )
        return 0;
    // free(pkemctx->key);
    pkemctx->kem = vkem;

    return 1;
}

static int kbc_enc_init(void *vpkemctx, void *vkem,
                               const OSSL_PARAM params[]) {
    return kbc_encdec_init(vpkemctx, vkem, EVP_PKEY_OP_ENCRYPT); // oqs 참고한거고 대칭키 쪽에는 EVP_PKEY_OP_ENCRYPT와 같은 상수가 없어서 그냥 공개키 쪽 가져다 씀 
}

static int kbc_dec_init(void *vpkemctx, void *vkem,
                               const OSSL_PARAM params[]) {
    return kbc_encdec_init(vpkemctx, vkem, EVP_PKEY_OP_DECRYPT); // The same as above
}
