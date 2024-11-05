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
#include <openssl/err.h>
#include <openssl/objects.h>
#include <openssl/params.h>
#include <openssl/provider.h>
#include <stdio.h>
#include <string.h>
#include "aes.h"

// aes example
static OSSL_FUNC_cipher_newctx_fn kbc_newctx;
static OSSL_FUNC_cipher_encrypt_init_fn kbc_aes128_init;
static OSSL_FUNC_cipher_update_fn kbc_aes128_update;
static OSSL_FUNC_cipher_final_fn kbc_aes128_final;
OSSL_DISPATCH openkbc[] = {
    {OSSL_FUNC_CIPHER_NEWCTX, (void (*)(void))kbc_newctx},
    {OSSL_FUNC_CIPHER_ENCRYPT_INIT, (void (*)(void))kbc_aes128_init},
    {OSSL_FUNC_CIPHER_UPDATE, (void (*)(void))kbc_aes128_update},
    {OSSL_FUNC_CIPHER_FINAL, (void (*)(void))kbc_aes128_final}

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

/* reference
static int oqs_kem_decapsencaps_init(void *vpkemctx, void *vkem,
                                     int operation) {
    PROV_OQSKEM_CTX *pkemctx = (PROV_OQSKEM_CTX *)vpkemctx;

    OQS_KEM_PRINTF3("OQS KEM provider called: _init : New: %p; old: %p \n",
                    vkem, pkemctx->kem);
    if (pkemctx == NULL || vkem == NULL || !oqsx_key_up_ref(vkem))
        return 0;
    oqsx_key_free(pkemctx->kem);
    pkemctx->kem = vkem;

    return 1;
}

static int oqs_kem_encaps_init(void *vpkemctx, void *vkem,
                               const OSSL_PARAM params[]) {
    OQS_KEM_PRINTF("OQS KEM provider called: encaps_init\n");
    return oqs_kem_decapsencaps_init(vpkemctx, vkem, EVP_PKEY_OP_ENCAPSULATE);
}

static int oqs_kem_decaps_init(void *vpkemctx, void *vkem,
                               const OSSL_PARAM params[]) {
    OQS_KEM_PRINTF("OQS KEM provider called: decaps_init\n");
    return oqs_kem_decapsencaps_init(vpkemctx, vkem, EVP_PKEY_OP_DECAPSULATE);
}
*/