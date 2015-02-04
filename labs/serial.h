#pragma once

#include "util/io.h"

namespace serial{

  static inline bool is_transmitter_ready(io_t baseport){
	
    return (io::read8(baseport,5)/32)%2;

  }
  static inline void writechar(uint8_t c, io_t baseport){

	io::write8(baseport,0,c);

  }

} //end serial
