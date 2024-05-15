void my_memcpy(uint8_t* dest, uint8_t* src, uint32_t len)
{
   uint32_t i, dend, send, ddata, sdata;

   for (i = 0; i < len; i++)
   {
      dend = (((uint32_t)dest) & 3) << 3;
      send = (((uint32_t)src ) & 3) << 3; 
      sdata = (((*(uint32_t*)(((uint32_t)src ) & 0xFFFFFFFC)) >> send) & 255) << dend;
      ddata =   (*(uint32_t*)(((uint32_t)dest) & 0xFFFFFFFC)) & (~(255 << dend));
      (*(uint32_t*)(((uint32_t)dest) & 0xFFFFFFFC)) = sdata | ddata;
      dest++;
      src++;
   }
}
