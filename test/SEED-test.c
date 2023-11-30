#include <stdio.h>
#include "seed.h"

int main() {
    /***********************test vector*********************/
    word Key[4] = { 0, };
    byte Plaintext[16] = { 0x03, 0x02, 0x01, 0x00, 0x07, 0x06, 0x05, 0x04, 0x0B, 0x0A, 0x09, 0x08, 0x0F ,0x0E, 0x0D, 0x0C };
    byte Plaintext2[16] = { 0, };

    byte Ciphertext[16] = { 0, };
    //expect encryption result: e0 c6 ba 5e 68 16 4e 05 cc f1 af 19 db 6c 34 6d
    SEED_Enc((word *)Ciphertext, (word *)Plaintext, Key);
    SEED_Dec((word *)Plaintext2, (word *)Ciphertext, Key);
    
    for (int i = 0; i < 16; i++)
        printf("%02x ", Ciphertext[i]);
    printf("\n");
    for (int i = 0; i < 16; i++)
        printf("%02x ", Plaintext2[i]);
    printf("\n");
}