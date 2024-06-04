#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
                                        INCLUDE FILES
==================================================================================================*/


/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
// Module ID for status codes.
#define MODULE_ID MAKE_MODULE_ID('a', 'e', 's')

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/
/**
 * Change Endian
 */

static uint32_t little_to_big_endian(uint32_t val) {
  return ((val >> 24) & 0xFF) | ((val & 0xFF00) << 8) | ((val >> 8) & 0xFF00) |
         ((val & 0xFF) << 24);
}

// Check that cipher mode enum from AES driver matches the one from the
// top-level API.
OT_ASSERT_ENUM_VALUE(kAesCipherModeEcb, (uint32_t)kBlockCipherModeEcb);
OT_ASSERT_ENUM_VALUE(kAesCipherModeCbc, (uint32_t)kBlockCipherModeCbc);
OT_ASSERT_ENUM_VALUE(kAesCipherModeCfb, (uint32_t)kBlockCipherModeCfb);
OT_ASSERT_ENUM_VALUE(kAesCipherModeOfb, (uint32_t)kBlockCipherModeOfb);
OT_ASSERT_ENUM_VALUE(kAesCipherModeCtr, (uint32_t)kBlockCipherModeCtr);

// Check GHASH context size against the underlying implementation.
static_assert(sizeof(ghash_context_t) == sizeof(gcm_ghash_context_t),
              "Sizes of GHASH context object for top-level API must match the "
              "underlying implementation.");

crypto_status_t otcrypto_aes_keygen(crypto_blinded_key_t *key) {
  // TODO: Implement AES sideloaded key generation once we have a keymgr
  // driver. In the meantime, non-sideloaded AES keys can simply be generated
  // using the DRBG and key-import functions.
  return kCryptoStatusNotImplemented;
}

/**
 * Extract an AES key from the blinded key struct.
 *
 * Also performs integrity, mode, and null-pointer checks on the key. This
 * function is only for basic AES modes; do not use for AES-GCM or AES-KWP keys
 * since they will fail the mode check.
 *
 * @param blinded_key Blinded key struct.
 * @param aes_mode Block cipher mode.
 * @param[out] aes_key Destination AES key struct.
 * @return Result of the operation.
 */
static status_t aes_key_construct(const crypto_blinded_key_t *blinded_key,
                                  const block_cipher_mode_t aes_mode,
                                  aes_key_t *aes_key) {
  // Key integrity check.
  if (launder32(integrity_blinded_key_check(blinded_key)) !=
      kHardenedBoolTrue) {
    return OTCRYPTO_BAD_ARGS;
  }
  HARDENED_CHECK_EQ(integrity_blinded_key_check(blinded_key),
                    kHardenedBoolTrue);

  // TODO(#15590): add support for sideloaded keys by actuating keymgr here if
  // needed (this requires a keymgr driver).
  if (blinded_key->config.hw_backed != kHardenedBoolFalse) {
    return OTCRYPTO_NOT_IMPLEMENTED;
  }
  aes_key->sideload = kHardenedBoolFalse;

  // Check for null pointer (not allowed for non-sideloaded keys).
  if (blinded_key->keyblob == NULL) {
    return OTCRYPTO_BAD_ARGS;
  }

  // Set the block cipher mode based on the key mode.
  switch (blinded_key->config.key_mode) {
    case kKeyModeAesEcb:
      aes_key->mode = kAesCipherModeEcb;
      break;
    case kKeyModeAesCbc:
      aes_key->mode = kAesCipherModeCbc;
      break;
    /*case kKeyModeAesCfb:
      aes_key->mode = kAesCipherModeCfb;
      break;
    case kKeyModeAesOfb:
      aes_key->mode = kAesCipherModeOfb;
      break;*/
    case kKeyModeAesCtr:
      aes_key->mode = kAesCipherModeCtr;
      break;
    default:
      return OTCRYPTO_BAD_ARGS;
  }

  // Check that the key mode matches the requested block cipher mode.
  if (memcmp(&aes_key->mode, &aes_mode, sizeof(aes_key->mode)) != 0) {
    return OTCRYPTO_BAD_ARGS;
  }
  HARDENED_CHECK_EQ(aes_key->mode, aes_mode);

  // Set the AES key length (in words).
  aes_key->key_len = keyblob_share_num_words(blinded_key->config);

  // Get pointers to the individual shares.
  uint32_t *share0;
  uint32_t *share1;
  HARDENED_TRY(keyblob_to_shares(blinded_key, &share0, &share1));
  aes_key->key_shares[0] = share0;
  aes_key->key_shares[1] = share1;

  return OTCRYPTO_OK;
}

/**
 * Applies the specified mode of AES padding to the block.
 *
 * Modifies only positions on and after index `last_block_len` in the
 * `padded_block`; real input may be written to the initial locations either
 * before or after calling this function.
 *
 * @param padding_mode Padding mode.
 * @param partial_data_len Length of real input in this block (may be 0).
 * @param[out] padded_block Destination padded block.
 * @return Result of the operation.
 */
static status_t aes_padding_apply(aes_padding_t padding_mode,
                                  const size_t partial_data_len,
                                  aes_block_t *block) {
  if (partial_data_len >= kAesBlockNumBytes) {
    return OTCRYPTO_BAD_ARGS;
  }

  // Get a byte-sized pointer to the padding start point within the block's
  // data buffer.
  char *padding = ((char *)block->data) + partial_data_len;

  // Pad a full block if the last block is full, otherwise just fill the last
  // block.
  size_t padding_len = kAesBlockNumBytes - partial_data_len;
  hardened_bool_t padding_written = kHardenedBoolFalse;
  switch (launder32(padding_mode)) {
    case kAesPaddingPkcs7:
      // Pads with value same as the number of padding bytes.
      memset(padding, padding_len, padding_len);
      padding_written = kHardenedBoolTrue;
      break;
    case kAesPaddingIso9797M2:
      // Pads with 0x80 (0b10000000), followed by zero bytes.
      memset(padding, 0x0, padding_len);
      padding[0] = 0x80;
      padding_written = kHardenedBoolTrue;
      break;
    case kAesPaddingNull:
      // This routine should not be called if padding is not needed.
      // return OTCRYPTO_RECOV_ERR;
      padding_written = kHardenedBoolTrue;
      break;
    default:
      // Unrecognized padding mode.
      return OTCRYPTO_BAD_ARGS;
  }
  HARDENED_CHECK_EQ(padding_written, kHardenedBoolTrue);

  return OTCRYPTO_OK;
}

/**
 * Return the number of blocks for the input and padding mode.
 *
 * @param plaintext_len Length of plaintext in bytes.
 * @param padding Padding mode.
 * @returns Number of AES blocks required.
 */
static status_t num_padded_blocks_get(size_t plaintext_len,
                                      aes_padding_t padding,
                                      size_t *num_blocks) {
  size_t num_full_blocks = plaintext_len / kAesBlockNumBytes;

  if (padding == kAesPaddingNull) {
    // If no padding mode is given, the last block must be full.
    if (num_full_blocks * kAesBlockNumBytes != plaintext_len) {
      return OTCRYPTO_BAD_ARGS;
    }

    *num_blocks = num_full_blocks;
    return OTCRYPTO_OK;
  }
  HARDENED_CHECK_NE(padding, kAesPaddingNull);

  // For non-null padding modes, we append a full block of padding if the last
  // block is full, so the value is always <full blocks> + 1.
  *num_blocks = num_full_blocks + 1;
  return OTCRYPTO_OK;
}

/**
 * Return the block at index i for the given input and padding mode.
 *
 * @param input Input data buffer.
 * @param padding Padding mode.
 * @param index Block index.
 * @param[out] AES block at `index`.
 * @param input_len Length of cipher input in bytes.
 * @param padding Padding mode.
 * @returns Number of AES blocks required.
 */

static status_t get_block(crypto_const_uint8_buf_t input, aes_padding_t padding,
                          size_t index, aes_block_t *block) {
  size_t num_full_blocks = input.len / kAesBlockNumBytes;

  // The index should never be more than `num_full_blocks` + 1, even including
  // padding.
  // HARDENED_CHECK_LE(index, num_full_blocks + 1);

  if (launder32(index) < num_full_blocks) {
    // HARDENED_CHECK_LT(index, num_full_blocks);
    // No need to worry about padding, just copy the data into the output
    // block.
    // TODO(#17711) Change to `hardened_memcpy`.
    memcpy(block->data, &input.data[index * kAesBlockNumBytes],
           kAesBlockNumBytes);
    return OTCRYPTO_OK;
  }
  // HARDENED_CHECK_GE(index, num_full_blocks);

  // If we get here, this block is the one with padding. It may be a partial
  // block or an empty block that will be entirely filled with padded bytes.
  size_t partial_data_len = input.len % kAesBlockNumBytes;
  memcpy(block->data, &input.data[index * kAesBlockNumBytes], partial_data_len);

  // Apply padding.
  HARDENED_TRY(aes_padding_apply(padding, partial_data_len, block));
  return OTCRYPTO_OK;
}
// NIF LX34300
crypto_status_t nifcrypto_aes_padded_plaintext_length(size_t plaintext_len,
                                                      aes_padding_t aes_padding,
                                                      size_t *padded_len ) {
  size_t padded_nblocks;
  if (num_padded_blocks_get(plaintext_len, aes_padding, &padded_nblocks).value != OTCRYPTO_OK.value)
    return kCryptoStatusBadArgs;
  *padded_len = padded_nblocks * kAesBlockNumBytes;
  return kCryptoStatusOK;
}

// NIF LX34300
crypto_status_t nifcrypto_aes(const crypto_blinded_key_t *	key,
                              crypto_word32_buf_t 		iv,
                              block_cipher_mode_t 		aes_mode,
                              aes_operation_t 		aes_operation,
                              crypto_const_byte_buf_t 	cipher_input,
                              aes_padding_t 			aes_padding,
                              crypto_byte_buf_t* 		cipher_output) {
  // Check for NULL pointers in input pointers and data buffers.
  if (key == NULL || (aes_mode != kBlockCipherModeEcb && iv.data == NULL) || cipher_input.data == NULL ||
      cipher_output->data == NULL) {
    return kCryptoStatusBadArgs;
  }

  // Calculate the number of blocks for the input, including the padding for
  // encryption.
  size_t input_nblocks;
  if (aes_operation == kAesOperationEncrypt) {
    if (aes_padding == kAesPaddingNull && aes_mode == kBlockCipherModeCtr &&
        cipher_input.len % kAesBlockNumBytes)
      input_nblocks = cipher_input.len / kAesBlockNumBytes + 1;
    else
      if (num_padded_blocks_get(cipher_input.len, aes_padding, &input_nblocks).value != OTCRYPTO_OK.value)
        return kCryptoStatusBadArgs;
  } else if (aes_operation == kAesOperationDecrypt) {
    // If the operation is decryption, the input length must be divisible by
    // the block size.
    if (launder32(cipher_input.len) % kAesBlockNumBytes != 0 &&
        aes_padding != kAesPaddingNull &&
        aes_mode != kBlockCipherModeCtr) {
      return kCryptoStatusBadArgs;
    }

    if (launder32(cipher_input.len) % kAesBlockNumBytes != 0 &&
        aes_padding == kAesPaddingNull &&
        aes_mode == kBlockCipherModeCtr) {  // CTR & Null padding but input size
                                            // is not multiple of block size
      input_nblocks = cipher_input.len / kAesBlockNumBytes + 1;
    } else {
      if(cipher_input.len % kAesBlockNumBytes == 0)
        input_nblocks = cipher_input.len / kAesBlockNumBytes;
    }
  }

  // Check for bad lengths:
  //   - IV must be exactly one block long.
  //   - Input length must be nonzero.
  //   - Output length must match number of input blocks.
  if (launder32(iv.len) != kAesBlockNumBytes || cipher_input.len == 0) {
    return kCryptoStatusBadArgs;
  }
  if ((aes_padding == kAesPaddingNull) && (aes_mode == kBlockCipherModeCtr)) {
    cipher_output->len = cipher_input.len;  // CTR mode + NULL padding
  } else if (cipher_input.len == 0 ||
             ((cipher_input.len % kAesBlockNumBytes != 0) &&
              (aes_padding == kAesPaddingNull) &&
              (aes_mode != kBlockCipherModeCtr)) ||
             launder32(cipher_output->len) !=
                 input_nblocks * kAesBlockNumBytes) {
    return kCryptoStatusBadArgs;
  }

  if (iv.len != kAesBlockNumBytes)
    return kCryptoStatusBadArgs;

  if (aes_padding != kAesPaddingNull && aes_mode != kBlockCipherModeCtr)
    if(cipher_output->len != input_nblocks *kAesBlockNumBytes)
      return kCryptoStatusBadArgs;// output len, input size block is same?

  // Load the IV.
  aes_block_t aes_iv;
  // TODO(#17711) Change to `hardened_memcpy`.
  memcpy(aes_iv.data, iv.data, kAesBlockNumBytes);

  // Parse the AES key.
  
  aes_key_t aes_key;
  if (aes_key_construct(key, aes_mode, &aes_key).value != OTCRYPTO_OK.value)
    return kCryptoStatusBadArgs;
  
  block_cipher_mode_t aes_mode_tmp = aes_mode;
  aes_mode = kAesCipherModeEcb;

  /****************************************/
  /* ctr mode is only using AES Encryption*/
  /****************************************/
  int depadd = 0;
  if (aes_mode_tmp == kAesCipherModeCtr) {
    if (aes_operation == kAesOperationDecrypt)
        depadd = 1;
    aes_operation = kAesOperationEncrypt;    
  }
  
  // Start the operation (encryption or decryption).
  switch (aes_operation) {
    case kAesOperationEncrypt:
      if (aes_encrypt_begin(aes_key, &aes_iv).value != OTCRYPTO_OK.value)
        return kCryptoStatusBadArgs;
      break;
    case kAesOperationDecrypt:
      if (aes_decrypt_begin(aes_key, &aes_iv).value != OTCRYPTO_OK.value)
        return kCryptoStatusBadArgs;
      break;
    default:
      return kCryptoStatusBadArgs;
  }

  // Perform the cipher operation for all full blocks (excluding last block).
  // The input and output are offset by one, so if unrolled this loop would
  // look like:
  //   aes_update(NULL, input[0]);
  //   aes_update(output[0], input[1]);
  //   aes_update(output[1], input[2]);
  //   ...
  // See the AES driver for details.
  aes_block_t block_in;
  aes_block_t block_out;
  
  size_t i;
  uint32_t temp[4];
  uint32_t temp2[4];
  aes_block_t block_in_tmp;

  // mode of operation
  if (aes_mode_tmp == kAesCipherModeEcb) {
    for (i = 0; launder32(i) < input_nblocks; i++) {
      get_block(cipher_input, aes_padding, i, &block_in);
      // Call the AES cipher and copy data to output buffer if needed.
      if (launder32(i) == 0) {
        
        if (aes_update(/*dest=*/NULL, &block_in).value != OTCRYPTO_OK.value)
          return kCryptoStatusInternalError;
      } else {
        if (aes_update(&block_out, &block_in).value != OTCRYPTO_OK.value)
          return kCryptoStatusInternalError;
        // TODO(#17711) Change to `hardened_memcpy`.
        
        memcpy(&cipher_output->data[(i - 1) * kAesBlockNumBytes], block_out.data,
               kAesBlockNumBytes);
      }
    }
  } else if (aes_mode_tmp == kAesCipherModeCbc && aes_operation == kAesOperationEncrypt) {
    memcpy(temp, aes_iv.data, 16);
    for (i = 0; launder32(i) < input_nblocks; i++) {
      get_block(cipher_input, aes_padding, i, &block_in);
      
      // Call the AES cipher and copy data to output buffer if needed.
      if (launder32(i) == 0) {
        block_in.data[0] ^= temp[0];
        block_in.data[1] ^= temp[1];
        block_in.data[2] ^= temp[2];
        block_in.data[3] ^= temp[3];
        if (aes_update(/*dest=*/NULL, &block_in).value != OTCRYPTO_OK.value)
          return kCryptoStatusInternalError;
      } else {
        if (aes_update(&block_out, NULL).value != OTCRYPTO_OK.value)
          return kCryptoStatusInternalError;
        // TODO(#17711) Change to `hardened_memcpy`.
        memcpy(temp, block_out.data, kAesBlockNumBytes);
        memcpy(&cipher_output->data[(i - 1) * kAesBlockNumBytes], block_out.data,
               kAesBlockNumBytes);
        block_in.data[0] ^= temp[0];
        block_in.data[1] ^= temp[1];
        block_in.data[2] ^= temp[2];
        block_in.data[3] ^= temp[3];
        if (aes_update(/*dest=*/NULL, &block_in).value != OTCRYPTO_OK.value)
          return kCryptoStatusInternalError; 
      }
    }
  } else if (aes_mode_tmp == kAesCipherModeCbc && aes_operation == kAesOperationDecrypt) {
    
    memcpy(temp, aes_iv.data, 16);

     for (i = 0; launder32(i) < input_nblocks; i++) {
      get_block(cipher_input, aes_padding, i, &block_in);
      
      // Call the AES cipher and copy data to output buffer if needed.
      if (launder32(i) == 0) {
        memcpy(temp2, block_in.data, kAesBlockNumBytes);
        if (aes_update(/*dest=*/NULL, &block_in).value != OTCRYPTO_OK.value)
          return kCryptoStatusInternalError;
      } else {
        if (aes_update(&block_out, NULL).value != OTCRYPTO_OK.value)
          return kCryptoStatusInternalError;
        if (i & 1) {
          block_out.data[0] ^= temp[0];
          block_out.data[1] ^= temp[1];
          block_out.data[2] ^= temp[2];
          block_out.data[3] ^= temp[3];
          memcpy(temp, block_in.data, kAesBlockNumBytes);
        }else {
          block_out.data[0] ^= temp2[0];
          block_out.data[1] ^= temp2[1];
          block_out.data[2] ^= temp2[2];
          block_out.data[3] ^= temp2[3];
          memcpy(temp2, block_in.data, kAesBlockNumBytes);
        }
        // TODO(#17711) Change to `hardened_memcpy`.
        memcpy(&cipher_output->data[(i - 1) * kAesBlockNumBytes], block_out.data,
               kAesBlockNumBytes);
        if (aes_update(/*dest=*/NULL, &block_in).value != OTCRYPTO_OK.value)
          return kCryptoStatusInternalError; 
      }
    }
  } else if (aes_mode_tmp == kAesCipherModeCtr) {
    
    temp[0] = aes_iv.data[0];
    temp[1] = aes_iv.data[1];
    temp[2] = aes_iv.data[2];
    uint32_t ctr = little_to_big_endian(aes_iv.data[3]);
    
    for (i = 0; launder32(i) < input_nblocks; i++) {
      get_block(cipher_input, aes_padding, i, &block_in);
      // Call the AES cipher and copy data to output buffer if needed.
       
      if (launder32(i) == 0) {
        temp[3] = little_to_big_endian(ctr++);

        memcpy(block_in_tmp.data, block_in.data, kAesBlockNumBytes);
        if (aes_update(/*dest=*/NULL, (aes_block_t *)&temp).value != OTCRYPTO_OK.value)
          return kCryptoStatusInternalError;
      } else {
        
        if (aes_update(&block_out, NULL).value != OTCRYPTO_OK.value)
          return kCryptoStatusInternalError;
        
        // TODO(#17711) Change to `hardened_memcpy`.
        block_out.data[0] ^= block_in_tmp.data[0];
        block_out.data[1] ^= block_in_tmp.data[1];
        block_out.data[2] ^= block_in_tmp.data[2];
        block_out.data[3] ^= block_in_tmp.data[3];

        memcpy(block_in_tmp.data, block_in.data, kAesBlockNumBytes);
        memcpy(&cipher_output->data[(i - 1) * kAesBlockNumBytes], block_out.data,
               kAesBlockNumBytes);
        temp[3] = little_to_big_endian(ctr++);
        if (aes_update(/*dest=*/NULL, (aes_block_t *)&temp).value != OTCRYPTO_OK.value)
          return kCryptoStatusInternalError;
      }
    }
  }

  // Check that the loop ran for the correct number of iterations.
  if (i != input_nblocks)
    return kCryptoStatusInternalError;

  // Retrieve the output from the final block (providing no input).
  if (aes_update(&block_out, /*src=*/NULL).value != OTCRYPTO_OK.value)
    return kCryptoStatusInternalError;
  // mode of operation of last block
  if (aes_mode_tmp == kAesCipherModeCbc && aes_operation == kAesOperationDecrypt && i & 1) {
    block_out.data[0] ^= temp[0];
    block_out.data[1] ^= temp[1];
    block_out.data[2] ^= temp[2];
    block_out.data[3] ^= temp[3];
  }else if (aes_mode_tmp == kAesCipherModeCbc && aes_operation == kAesOperationDecrypt) {
    block_out.data[0] ^= temp2[0];
    block_out.data[1] ^= temp2[1];
    block_out.data[2] ^= temp2[2];
    block_out.data[3] ^= temp2[3];
  }else if (aes_mode_tmp == kAesCipherModeCtr) {
    block_out.data[0] ^= block_in_tmp.data[0];
    block_out.data[1] ^= block_in_tmp.data[1];
    block_out.data[2] ^= block_in_tmp.data[2];
    block_out.data[3] ^= block_in_tmp.data[3];
  }
  // TODO(#17711) Change to `hardened_memcpy`.
  memcpy(&cipher_output->data[(input_nblocks - 1) * kAesBlockNumBytes],
         block_out.data, kAesBlockNumBytes);
  // Deinitialize the AES block.
  if(aes_end().value != OTCRYPTO_OK.value)
    return kCryptoStatusInternalError;
  
  // depadding length when aes decrypt
  int cnt = 1;
  if (aes_operation == kAesOperationDecrypt || (aes_mode_tmp == kAesCipherModeCtr && depadd)) {
    switch (launder32(aes_padding)) {
      case kAesPaddingPkcs7:
        if ((cipher_output->data[cipher_output->len - 1] >= 1) &&
            (cipher_output->data[cipher_output->len - 1] <= 16))
          cipher_output->len -= cipher_output->data[cipher_output->len - 1];
        else  // Incorrect padding
          return kCryptoStatusBadArgs;
        break;
      case kAesPaddingIso9797M2:
        for (i = 1; i <= 16; i++) {
          if (cipher_output->data[cipher_output->len - i] == 0x80) {
            cipher_output->len -= cnt;
            break;
          } else if (cipher_output->data[cipher_output->len - i] == 0) {
            cnt++;
            if (cnt == 17)  // 17 bytes that are zero
              return kCryptoStatusBadArgs;
          } else  // Incorrect padding
            return kCryptoStatusBadArgs;
        }
        break;
      case kAesPaddingNull:
        // This routine should not be called if padding is not needed.
        // return kCryptoStatusBadArgs;
        break;
      default:
        // Unrecognized padding mode.
        return kCryptoStatusBadArgs;
    }
  }


  return kCryptoStatusOK;
}

#ifdef __cplusplus
}
#endif
