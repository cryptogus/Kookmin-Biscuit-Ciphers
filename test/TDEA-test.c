#include <stdio.h>
#include "tdes.h"

void print_data(uint8_t *data,int n)
{
  int i;
  for(i=0;i<n;i++) {
    printf("%02x ",data[i]);
  }
  printf("\n");
}

int cmp(uint8_t *data, uint8_t *data2, int n) {
    for (int i = 0; i < n; i++) {
        if (data[i] != data2[i])
            return 1;
    }
    return 0;
}

int main(void)
{
  /************************* test vector *************************/
  uint8_t key1[8]={0x5b,0x5a,0x57,0x67,0x6a,0x56,0x67,0x6e};
  uint8_t key2[8]={0x17,0x42,0x33,0x11,0x22,0x33,0x44,0x8e};
  uint8_t key3[8]={0x91,0xa2,0xcc,0x31,0x20,0xef,0xba,0x7e};

  uint8_t in[8]={0x67,0x5a,0x69,0x67,0x5e,0x5a,0x6b,0x5a};
  uint8_t outex[8]={0x97,0x4a,0xff,0xbf,0x86,0x02,0x2d,0x1f}; // in[8] + key1 => des encryption
  uint8_t out[8];

  /**
   * TEST 1
   * (key length = 16-byte), padding X
   * */

  uint8_t tkey[16]={0x5b,0x5a,0x57,0x67,0x6a,0x56,0x67,0x6e,
                    0x17,0x42,0x33,0x11,0x22,0x33,0x44,0x8e}; // key1 + key2
  uint8_t tout[8] = {0x69, 0xbf, 0x1b, 0xe0, 0xdd, 0x09, 0x58, 0xf8}; // ciphertext answer
  
  printf("*******************************************************\n");
  printf("*          TDES TEST, key length = 16-byte            *\n");
  printf("*******************************************************\n\n");
  
  TDES_CTX tdes;

  if (TDES_set_key(&tdes, (uint32_t *)tkey, sizeof(tkey)/sizeof(uint8_t)) != 1)
    fprintf(stderr,"Set key fail\n");
  
  printf("==================== TDES ENCRYPT ====================\n");
  TDES_ECB_Enc(&tdes,(uint32_t *)out,(uint32_t *)in, 8);
  printf("PLAINTEXT : ");
  print_data((uint8_t *)in, 8);
  printf("CIPHERTEXT: ");
  print_data((uint8_t *)out, 8);


  printf("==================== TDES DECRYPT1 ====================\n");
  
  printf("CIPHERTEXT: ");
  print_data((uint8_t *)out, 8);

  TDES_ECB_Dec(&tdes,(uint32_t *)out,(uint32_t *)out, 8);

  printf("PLAINTEXT : ");
  print_data((uint8_t *)out, 8);

  /**
   * TEST 2
   * (key length = 24-byte)
   * */
  uint8_t tkey2[24]={0x5b,0x5a,0x57,0x67,0x6a,0x56,0x67,0x6e,
                    0x17,0x42,0x33,0x11,0x22,0x33,0x44,0x8e,
                    0x91,0xa2,0xcc,0x31,0x20,0xef,0xba,0x7e}; // key1 + key2 + key3
  uint8_t tout2[8] = {0x11, 0x28, 0xa4, 0xc2, 0xa0, 0xe3, 0x36, 0x81}; // ciphertext answer
  
  printf("\n*******************************************************\n");
  printf("*          TDES TEST, key length = 24-byte            *\n");
  printf("*******************************************************\n\n");

  printf("==================== TDES ENCRYPT2 ====================\n");

  if (TDES_set_key(&tdes, (uint32_t *)tkey2, sizeof(tkey2)/sizeof(uint8_t)) != 1)
    fprintf(stderr,"Set key fail\n");

  TDES_ECB_Enc(&tdes,(uint32_t *)out,(uint32_t *)in, 8);
  printf("PLAINTEXT : ");
  print_data((uint8_t *)&in, 8);
  printf("CIPHERTEXT: ");
  print_data((uint8_t *)&out, 8);

  printf("==================== TDES DECRYPT2 ====================\n");
  
  printf("CIPHERTEXT: ");
  print_data((uint8_t *)out, 8);

  TDES_ECB_Dec(&tdes,(uint32_t *)out,(uint32_t *)out, 8);

  printf("PLAINTEXT : ");
  print_data((uint8_t *)out, 8);

  return 0;
}