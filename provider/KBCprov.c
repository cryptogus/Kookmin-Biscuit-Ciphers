// SPDX-License-Identifier: Apache-2.0 AND MIT

/*
 * OQS OpenSSL 3 provider
 *
 * Code strongly inspired by OpenSSL legacy provider.
 *
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
