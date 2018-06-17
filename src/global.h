#ifndef _VVC_GLOBAL_H
#define _VVC_GLOBAL_H

#include <stdint.h>
#ifdef VVC_F0
  #include "stm32f0xx.h"
#elif  VVC_L0
  #include "stm32l0xx.h"
#endif

// Global variables.
uint32_t core_clock_hz;

#endif
