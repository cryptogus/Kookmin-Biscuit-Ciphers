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

// aes example
static OSSL_FUNC_cipher_newctx_fn kbc_newctx;
OSSL_DISPATCH openkbc[] = {
    {OSSL_FUNC_CIPHER_NEWCTX, kbc_newctx},

};