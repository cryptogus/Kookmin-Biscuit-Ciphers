#pragma once

#include <string.h>
#include <stdlib.h>

unsigned char *pkcs7_padding(unsigned char *input, size_t block_size, size_t *len);
unsigned char *pkcs7_depadding(unsigned char *input, size_t *len);