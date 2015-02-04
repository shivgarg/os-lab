#include "util/io.h"

namespace vgatext{

   static inline void writechar(int loc, uint8_t c, uint8_t bg, uint8_t fg, addr_t base){

      mmio::write16(base,(uint16_t)2*loc,(uint16_t)((((int)c))+((int)fg*256)+((int)bg)*4096));

   }

}//namespace vgatext
