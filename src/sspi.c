#include "sspi.h"

/*
 * Write a byte of data using software SPI. For each bit:
 * 1. Pull the clock pin low.
 * 2. Set the 'MOSI' data pin to the correct value.
 * 3. Pull the clock pin high.
 */
inline void sspi_w(uint8_t dat) {
  uint8_t sspi_i;
  // Send 8 bits, with the MSB first.
  for (sspi_i = 0x80; sspi_i != 0x00; sspi_i >>= 1) {
    GPIOB->ODR &= ~(1 << PB_SCK);
    if (dat & sspi_i) {
      GPIOB->ODR |=  (1 << PB_MOSI);
    }
    else {
      GPIOB->ODR &= ~(1 << PB_MOSI);
    }
    GPIOB->ODR |=  (1 << PB_SCK);
  }
}

/*
 * Write a 'Command byte' over software SPI.
 * "4-wire" SPI interfaces are common in devices like
 * screens. A 'Data/Command' pin determines whether
 * the SPI data is an instruction for the display,
 * or pixel data for the framebuffer RAM.
 * Here, '0' means 'Command' and '1' means 'Data'.
 */
inline void sspi_cmd(uint8_t cdat) {
  // Pull the 'D/C' pin low, write data, pull 'D/C' high.
  GPIOB->ODR &= ~(1 << PB_DC);
  sspi_w(cdat);
  GPIOB->ODR |=  (1 << PB_DC);
}

/*
 * Initialize the SPI peripheral for use with a 4-wire display.
 */
void hspi_init(SPI_TypeDef *SPIx) {
  // Ensure that the peripheral is disabled, and reset it.
  SPIx->CR1 &= ~(SPI_CR1_SPE);
  if (SPIx == SPI1) {
    RCC->APB2RSTR |=  (RCC_APB2RSTR_SPI1RST);
    RCC->APB2RSTR &= ~(RCC_APB2RSTR_SPI1RST);
  }
  // Use unidirectional simplex mode.
  //SPIx->CR1 &= ~(SPI_CR1_BIDIMODE |
  //               SPI_CR1_BIDIOE);
  // Set clock polarity/phase to 0/0?
  SPIx->CR1 &= ~(SPI_CR1_CPOL |
                 SPI_CR1_CPHA);
  //SPIx->CR1 |=  (SPI_CR1_CPHA);
  // Or 1/1 seems to work...
  SPIx->CR1 |=  (SPI_CR1_CPOL |
                 SPI_CR1_CPHA);
  // Set the STM32 to act as a host device.
  SPIx->CR1 |=  (SPI_CR1_MSTR);
  // Set software 'Chip Select' pin.
  SPIx->CR1 |=  (SPI_CR1_SSM);
  // (Set the internal 'Chip Select' signal.)
  SPIx->CR1 |=  (SPI_CR1_SSI);
  // Use MSB-first format.
  SPIx->CR1 &= ~(SPI_CR1_LSBFIRST);
  // I think that these bits are required for some reason.
  //SPIx->CR2 |=  (SPI_CR2_SSOE);
  // Set 8 bits per frame.
  #ifdef VVC_F0
    // (The F0 series features configurable frame width.)
    SPIx->CR2 &= ~(SPI_CR2_DS);
    SPIx->CR2 |=  (0x7 << SPI_CR2_DS_Pos);
  #elif  VVC_L0
    // (The L0 series only supports 8- and 16-bit frames.)
    SPIx->CR1 &= ~(SPI_CR1_DFF);
  #endif
  // Set the Baud rate prescaler.
  SPIx->CR1 &= ~(SPI_CR1_BR);
  // Start slow? SPI_clock = Core_clock / (2 ^ (BR))
  // So, a value of 4 should slow things down by a factor of 16.
  //SPIx->CR1 |=  (0x4 << SPI_CR1_BR_Pos);
  // Enable the peripheral.
  SPIx->CR1 |=  (SPI_CR1_SPE);
}

/*
 * Send a byte of data over hardware SPI.
 * This method does not wait for the communication to finish.
 * (The STM32 has an onboard FIFO queue, so we can check
 *  whether that has space for writing instead.)
 */
inline void hspi_w8(SPI_TypeDef *SPIx, uint8_t dat) {
  // Wait for TXE.
  while (!(SPIx->SR & SPI_SR_TXE)) {};
  // Send the byte.
  *(uint8_t*)&(SPIx->DR) = dat;
}

/*
 * Send 16 bits of data over hardware SPI.
 * This method does not wait for the communication to finish.
 * It adds two bytes to the FIFO queue at once; that's one
 * pixel for a 16-bit color display.
 *
 * On L0 platforms, packing 2 data frames at once does
 * not appear to be supported.
 */
inline void hspi_w16(SPI_TypeDef *SPIx, uint16_t dat) {
#ifdef VVC_F0
  // Wait for TXE.
  while (!(SPIx->SR & SPI_SR_TXE)) {};
  // Send the data.
  // (Flip the bytes for the little-endian ARM core.)
  dat = (((dat & 0x00FF) << 8) | ((dat & 0xFF00) >> 8));
  *(uint16_t*)&(SPIx->DR) = dat;
#elif  VVC_L0
  hspi_w8(SPIx, (uint8_t)(dat >> 8));
  hspi_w8(SPIx, (uint8_t)(dat & 0xFF));
#endif
}

/*
 * Send a 'command' byte over hardware SPI.
 * Pull the 'D/C' pin low, send the byte, then pull the pin high.
 * Wait for the transmission to finish before changing the
 * 'D/C' pin value.
 */
inline void hspi_cmd(SPI_TypeDef *SPIx, uint8_t cmd) {
  while ((SPIx->SR & SPI_SR_BSY)) {};
  GPIOB->ODR &= ~(1 << PB_DC);
  hspi_w8(SPIx, cmd);
  while ((SPIx->SR & SPI_SR_BSY)) {};
  GPIOB->ODR |=  (1 << PB_DC);
}
