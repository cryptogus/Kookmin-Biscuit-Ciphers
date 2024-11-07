/**
 * @file KBCprov.c
 * @brief Example of how to load oqsprovider when compiled as a static library
 *        `using OSSL_PROVIDER_add_builtin`.
 */

/**
 * @ref https://github.com/openssl/openssl/blob/master/README-PROVIDERS.md
 * @ref https://github.com/open-quantum-safe/oqs-provider/blob/main/examples/static_oqsprovider.c
 * @ref https://github.com/open-quantum-safe/oqs-provider/blob/main/CONFIGURE.md
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

/** @brief The initialization function of oqsprovider. */
extern OSSL_provider_init_fn kbc_provider_init;

/** @brief Name of the oqsprovider. */
static const char *kOQSProviderName = "kbcprovider";

/**
 *
 * @param libctx Context of the OpenSSL library in which to load the
 * kbcprovider.
 *
 * @return 0 if success, else -1. */
static int load_oqs_provider(OSSL_LIB_CTX *libctx)
{
    OSSL_PROVIDER *provider;
    int ret;

    ret = OSSL_PROVIDER_available(libctx, kOQSProviderName);
    if (ret != 0) {
        fprintf(stderr,
                "`OSSL_PROVIDER_available` returned %i, but 0 was expected\n",
                ret);
        return -1;
    }

    ret = OSSL_PROVIDER_add_builtin(libctx, kOQSProviderName,
                                    kbc_provider_init);
    if (ret != 1) {
        fprintf(stderr,
                "`OSSL_PROVIDER_add_builtin` failed with returned code %i\n",
                ret);
        return -1;
    }

    provider = OSSL_PROVIDER_load(libctx, kOQSProviderName);
    if (provider == NULL) {
        fputs("`OSSL_PROVIDER_load` failed\n", stderr);
        return -1;
    }

    ret = OSSL_PROVIDER_available(libctx, kOQSProviderName);
    if (ret != 1) {
        fprintf(stderr,
                "`OSSL_PROVIDER_available` returned %i, but 0 was expected\n",
                ret);
        return -1;
    }

    ret = OSSL_PROVIDER_self_test(provider);
    if (ret != 1) {
        fprintf(stderr,
                "`OSSL_PROVIDER_self_test` failed with returned code %i\n",
                ret);
        return -1;
    }

    return 0;
}

int main()
{
    OSSL_LIB_CTX *libctx;
    int ret;

    libctx = OSSL_LIB_CTX_new();
    if (libctx == NULL) {
        fputs("`OSSL_LIB_CTX_new` failed. Cannot initialize OpenSSL.\n",
              stderr);
        return 1;
    }

    ret = load_oqs_provider(libctx);
    if (ret != 0) {
        fputs("`load_oqs_provider` failed. Dumping OpenSSL error queue.\n",
              stderr);
        ERR_print_errors_fp(stderr);
        return 2;
    }

    OSSL_LIB_CTX_free(libctx);

    return 0;
}
