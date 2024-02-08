#include <stdio.h>
#include "chacha20.h"

int main(void) {
   uint32_t a = 0x11111111;
   uint32_t b = 0x01020304;
   uint32_t c = 0x9b8d6f43;
   uint32_t d = 0x01234567;

   printf("Test vector Input values\n");
   printf("a: %#08x b: %#08x c: %#08x d: %#08x\n\n",a, b, c, d);

   ChaCha_Quarter_Round(&a, &b, &c, &d);
   printf("After running a Quarter Round on these four numbers\no  a = 0xea2a92f4\no  b = 0xcb1cf8ce\no  c = 0x4581472e\no  d = 0x5881c4bb\n\n");
   printf("User's result: a: %#08x b: %#08x c: %#08x d: %#08x\n", a, b, c, d);
}