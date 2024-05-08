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
  uint8_t w[128];

  // test vector 1
  uint8_t key[8]={0x5b,0x5a,0x57,0x67,0x6a,0x56,0x67,0x6e};
  uint8_t in[8]={0x67,0x5a,0x69,0x67,0x5e,0x5a,0x6b,0x5a};
  uint8_t outex[8]={0x97,0x4a,0xff,0xbf,0x86,0x02,0x2d,0x1f};

  // test vector 2
  //uint8_t key[8]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  //uint8_t in[8]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  //uint8_t outex[8]={0x8c, 0xa6, 0x4d, 0xe9, 0xc1, 0xb1, 0x23, 0xa7};
  uint8_t out[8];

  keyexpansion(w,key);


  des_encrypt((uint32_t *)in,(uint32_t *)out,(uint32_t *)w);

  printf("==== Answer ciphertext =====\n");
  print_data(outex, 8);
  printf("==== Test cipher vector ====\n");
  print_data(out, 8);

  return 0;
}