#ifndef _VVC_UTIL_H
#define _VVC_UTIL_H

#include "global.h"
#include "sspi.h"

/* Utility method declarations. */
void ili9341_sspi_init(void);
void ili9341_hspi_init(SPI_TypeDef *SPIx);
void delay_cycles(uint32_t cyc);

#endif
