#pragma once

#include <string.h>
#include <stdlib.h>

unsigned char *pkcs7_padding(unsigned char *input, size_t block_size);