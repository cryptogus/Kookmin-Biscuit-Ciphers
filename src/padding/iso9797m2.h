#pragma once

#include <string.h>
#include <stdlib.h>

unsigned char *ios9797m2_padding(unsigned char *input, size_t block_size, size_t *len);
unsigned char *ios9797m2_depadding(unsigned char *input, size_t block_size, size_t *len);