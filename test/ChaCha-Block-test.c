#include <stdio.h>
#include "chacha20.h"
/*
   The ChaCha20 state is initialized as follows:

   o  The first four words (0-3) are constants: 0x61707865, 0x3320646e,
      0x79622d32, 0x6b206574.

   o  The next eight words (4-11) are taken from the 256-bit key by
      reading the bytes in little-endian order, in 4-byte chunks.

   o  Word 12 is a block counter.  Since each block is 64-byte, a 32-bit
      word is enough for 256 gigabytes of data.

   o  Words 13-15 are a nonce, which should not be repeated for the same
      key.  The 13th word is the first 32 bits of the input nonce taken
      as a little-endian integer, while the 15th word is the last 32
      bits.

       cccccccc  cccccccc  cccccccc  cccccccc
       kkkkkkkk  kkkkkkkk  kkkkkkkk  kkkkkkkk
       kkkkkkkk  kkkkkkkk  kkkkkkkk  kkkkkkkk
       bbbbbbbb  nnnnnnnn  nnnnnnnn  nnnnnnnn

 For a test vector, we will use the following inputs to the ChaCha20
   block function:

   o  Key = 00:01:02:03:04:05:06:07:08:09:0a:0b:0c:0d:0e:0f:10:11:12:13:
      14:15:16:17:18:19:1a:1b:1c:1d:1e:1f.  The key is a sequence of
      octets with no particular structure before we copy it into the
      ChaCha state.

   o  Nonce = (00:00:00:09:00:00:00:4a:00:00:00:00)

   o  Block Count = 1.
   
   After setting up the ChaCha state, it looks like this:

   ChaCha state with the key setup. (little-endian order)

       61707865  3320646e  79622d32  6b206574
       03020100  07060504  0b0a0908  0f0e0d0c
       13121110  17161514  1b1a1918  1f1e1d1c
       00000001  09000000  4a000000  00000000

   After running 20 rounds (10 column rounds interleaved with 10
   "diagonal rounds"), the ChaCha state looks like this:

   ChaCha state after 20 rounds

       837778ab  e238d763  a67ae21e  5950bb2f
       c4f2d0c7  fc62bb2f  8fa018fc  3f5ec7b7
       335271c2  f29489f3  eabda8fc  82e46ebd
       d19c12b4  b04e16de  9e83d0cb  4e3c50a2

   Finally, we add the original state to the result (simple vector or
   matrix addition), giving this:

   ChaCha state at the end of the ChaCha20 operation

       e4e7f110  15593bd1  1fdd0f50  c47120a3
       c7f4d1c7  0368c033  9aaa2204  4e6cd4c3
       466482d2  09aa9f07  05d7c214  a2028bd9
       d19c12b5  b94e16de  e883d0cb  4e3c50a2
*/
int main(void) {
   uint32_t block[16] = {0x61707865,  0x3320646e,  0x79622d32,  0x6b206574,
                         0x03020100,  0x07060504,  0x0b0a0908,  0x0f0e0d0c,
                         0x13121110,  0x17161514,  0x1b1a1918,  0x1f1e1d1c,
                         0x00000001,  0x09000000,  0x4a000000,  0x00000000};

   chacha20_block(block);
   printf("User result:\n");
   for (int i = 0; i < 16; i++) {
    if (i != 0 && i % 4 == 0)
            printf("\n");
        printf("0x%08x ", block[i]);
   }
   printf("\n\nAnswer is:\n");

   printf("0xe4e7f110 0x15593bd1 0x1fdd0f50 0xc47120a3\n0xc7f4d1c7 0x0368c033 0x9aaa2204 0x4e6cd4c3\n0x466482d2 0x09aa9f07 0x05d7c214 0xa2028bd9\n0xd19c12b5 0xb94e16de 0xe883d0cb 0x4e3c50a2\n");
}