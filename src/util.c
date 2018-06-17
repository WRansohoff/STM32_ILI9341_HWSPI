#include "util.h"

// Initialize a 320x240-pixel ILI9341 TFT display.
void ili9341_sspi_init(void) {
  // (Display off)
  sspi_cmd(0x28);

  // Issue a series of initialization commands from the
  // Adafruit library for a simple 'known good' test.
  sspi_cmd(0xEF);
  sspi_w(0x03);
  sspi_w(0x80);
  sspi_w(0x02);
  sspi_cmd(0xCF);
  sspi_w(0x00);
  sspi_w(0xC1);
  sspi_w(0x30);
  sspi_cmd(0xED);
  sspi_w(0x64);
  sspi_w(0x03);
  sspi_w(0x12);
  sspi_w(0x81);
  sspi_cmd(0xE8);
  sspi_w(0x85);
  sspi_w(0x00);
  sspi_w(0x78);
  sspi_cmd(0xCB);
  sspi_w(0x39);
  sspi_w(0x2C);
  sspi_w(0x00);
  sspi_w(0x34);
  sspi_w(0x02);
  sspi_cmd(0xF7);
  sspi_w(0x20);
  sspi_cmd(0xEA);
  sspi_w(0x00);
  sspi_w(0x00);
  // PWCTR1
  sspi_cmd(0xC0);
  sspi_w(0x23);
  // PWCTR2
  sspi_cmd(0xC1);
  sspi_w(0x10);
  // VMCTR1
  sspi_cmd(0xC5);
  sspi_w(0x3E);
  sspi_w(0x28);
  // VMCTR2
  sspi_cmd(0xC7);
  sspi_w(0x86);
  // MADCTL
  sspi_cmd(0x36);
  sspi_w(0x48);
  // VSCRSADD
  sspi_cmd(0x37);
  sspi_w(0x00);
  // PIXFMT
  sspi_cmd(0x3A);
  sspi_w(0x55);
  // FRMCTR1
  sspi_cmd(0xB1);
  sspi_w(0x00);
  sspi_w(0x18);
  // DFUNCTR
  sspi_cmd(0xB6);
  sspi_w(0x08);
  sspi_w(0x82);
  sspi_w(0x27);
  sspi_cmd(0xF2);
  sspi_w(0x00);
  // GAMMASET
  sspi_cmd(0x26);
  sspi_w(0x01);
  // (Actual gamma settings)
  sspi_cmd(0xE0);
  sspi_w(0x0F);
  sspi_w(0x31);
  sspi_w(0x2B);
  sspi_w(0x0C);
  sspi_w(0x0E);
  sspi_w(0x08);
  sspi_w(0x4E);
  sspi_w(0xF1);
  sspi_w(0x37);
  sspi_w(0x07);
  sspi_w(0x10);
  sspi_w(0x03);
  sspi_w(0x0E);
  sspi_w(0x09);
  sspi_w(0x00);
  sspi_cmd(0xE1);
  sspi_w(0x00);
  sspi_w(0x0E);
  sspi_w(0x14);
  sspi_w(0x03);
  sspi_w(0x11);
  sspi_w(0x07);
  sspi_w(0x31);
  sspi_w(0xC1);
  sspi_w(0x48);
  sspi_w(0x08);
  sspi_w(0x0F);
  sspi_w(0x0C);
  sspi_w(0x31);
  sspi_w(0x36);
  sspi_w(0x0F);

  // Exit sleep mode.
  // Display on.
  sspi_cmd(0x11);
  delay_cycles(2000000);
  sspi_cmd(0x29);
  delay_cycles(2000000);
}

void ili9341_hspi_init(SPI_TypeDef *SPIx) {
  // (Display off)
  //hspi_cmd(SPIx, 0x28);

  // Issue a series of initialization commands from the
  // Adafruit library for a simple 'known good' test.
  hspi_cmd(SPIx, 0xEF);
  hspi_w8(SPIx, 0x03);
  hspi_w8(SPIx, 0x80);
  hspi_w8(SPIx, 0x02);
  hspi_cmd(SPIx, 0xCF);
  hspi_w8(SPIx, 0x00);
  hspi_w8(SPIx, 0xC1);
  hspi_w8(SPIx, 0x30);
  hspi_cmd(SPIx, 0xED);
  hspi_w8(SPIx, 0x64);
  hspi_w8(SPIx, 0x03);
  hspi_w8(SPIx, 0x12);
  hspi_w8(SPIx, 0x81);
  hspi_cmd(SPIx, 0xE8);
  hspi_w8(SPIx, 0x85);
  hspi_w8(SPIx, 0x00);
  hspi_w8(SPIx, 0x78);
  hspi_cmd(SPIx, 0xCB);
  hspi_w8(SPIx, 0x39);
  hspi_w8(SPIx, 0x2C);
  hspi_w8(SPIx, 0x00);
  hspi_w8(SPIx, 0x34);
  hspi_w8(SPIx, 0x02);
  hspi_cmd(SPIx, 0xF7);
  hspi_w8(SPIx, 0x20);
  hspi_cmd(SPIx, 0xEA);
  hspi_w8(SPIx, 0x00);
  hspi_w8(SPIx, 0x00);
  // PWCTR1
  hspi_cmd(SPIx, 0xC0);
  hspi_w8(SPIx, 0x23);
  // PWCTR2
  hspi_cmd(SPIx, 0xC1);
  hspi_w8(SPIx, 0x10);
  // VMCTR1
  hspi_cmd(SPIx, 0xC5);
  hspi_w8(SPIx, 0x3E);
  hspi_w8(SPIx, 0x28);
  // VMCTR2
  hspi_cmd(SPIx, 0xC7);
  hspi_w8(SPIx, 0x86);
  // MADCTL
  hspi_cmd(SPIx, 0x36);
  hspi_w8(SPIx, 0x48);
  // VSCRSADD
  hspi_cmd(SPIx, 0x37);
  hspi_w8(SPIx, 0x00);
  // PIXFMT
  hspi_cmd(SPIx, 0x3A);
  hspi_w8(SPIx, 0x55);
  // FRMCTR1
  hspi_cmd(SPIx, 0xB1);
  hspi_w8(SPIx, 0x00);
  hspi_w8(SPIx, 0x18);
  // DFUNCTR
  hspi_cmd(SPIx, 0xB6);
  hspi_w8(SPIx, 0x08);
  hspi_w8(SPIx, 0x82);
  hspi_w8(SPIx, 0x27);
  hspi_cmd(SPIx, 0xF2);
  hspi_w8(SPIx, 0x00);
  // GAMMASET
  hspi_cmd(SPIx, 0x26);
  hspi_w8(SPIx, 0x01);
  // (Actual gamma settings)
  hspi_cmd(SPIx, 0xE0);
  hspi_w8(SPIx, 0x0F);
  hspi_w8(SPIx, 0x31);
  hspi_w8(SPIx, 0x2B);
  hspi_w8(SPIx, 0x0C);
  hspi_w8(SPIx, 0x0E);
  hspi_w8(SPIx, 0x08);
  hspi_w8(SPIx, 0x4E);
  hspi_w8(SPIx, 0xF1);
  hspi_w8(SPIx, 0x37);
  hspi_w8(SPIx, 0x07);
  hspi_w8(SPIx, 0x10);
  hspi_w8(SPIx, 0x03);
  hspi_w8(SPIx, 0x0E);
  hspi_w8(SPIx, 0x09);
  hspi_w8(SPIx, 0x00);
  hspi_cmd(SPIx, 0xE1);
  hspi_w8(SPIx, 0x00);
  hspi_w8(SPIx, 0x0E);
  hspi_w8(SPIx, 0x14);
  hspi_w8(SPIx, 0x03);
  hspi_w8(SPIx, 0x11);
  hspi_w8(SPIx, 0x07);
  hspi_w8(SPIx, 0x31);
  hspi_w8(SPIx, 0xC1);
  hspi_w8(SPIx, 0x48);
  hspi_w8(SPIx, 0x08);
  hspi_w8(SPIx, 0x0F);
  hspi_w8(SPIx, 0x0C);
  hspi_w8(SPIx, 0x31);
  hspi_w8(SPIx, 0x36);
  hspi_w8(SPIx, 0x0F);

  // Exit sleep mode.
  hspi_cmd(SPIx, 0x11);
  delay_cycles(2000000);
  // Display on.
  hspi_cmd(SPIx, 0x29);
  delay_cycles(2000000);
  // 'Normal' display mode.
  hspi_cmd(SPIx, 0x13);
}

// Simple delay method, with instructions not to optimize.
// It doesn't accurately delay a precise # of cycles,
// it's just a rough scale.
void __attribute__((optimize("O0"))) delay_cycles(uint32_t cyc) {
  uint32_t d_i;
  for (d_i = 0; d_i < cyc; ++d_i) {
    asm("NOP");
  }
}
