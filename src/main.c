#include "main.h"

/**
 * Main program.
 */
int main(void) {
  // Initial clock setup.
  #ifdef VVC_F0
    // Reset the Flash 'Access Control Register', and
    // then set 1 wait-state and enable the prefetch buffer.
    // (The device header files only show 1 bit for the F0
    //  line, but the reference manual shows 3...)
    FLASH->ACR &= ~(0x00000017);
    FLASH->ACR |=  (FLASH_ACR_LATENCY |
                    FLASH_ACR_PRFTBE);
    // Configure the PLL to (HSI / 2) * 12 = 48MHz.
    // Use a PLLMUL of 0xA for *12, and keep PLLSRC at 0
    // to use (HSI / PREDIV) as the core source. HSI = 8MHz.
    RCC->CFGR  &= ~(RCC_CFGR_PLLMUL |
                    RCC_CFGR_PLLSRC);
    RCC->CFGR  |=  (RCC_CFGR_PLLSRC_HSI_DIV2 |
                    RCC_CFGR_PLLMUL12);
    // Turn the PLL on and wait for it to be ready.
    RCC->CR    |=  (RCC_CR_PLLON);
    while (!(RCC->CR & RCC_CR_PLLRDY)) {};
    // Select the PLL as the system clock source.
    RCC->CFGR  &= ~(RCC_CFGR_SW);
    RCC->CFGR  |=  (RCC_CFGR_SW_PLL);
    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {};
    // The system clock is now 48MHz.
    core_clock_hz = 48000000;
  #elif VVC_L0
    // Set the Flash ACR to use 1 wait-state
    // and enable the prefetch buffer and pre-read.
    FLASH->ACR |=  (FLASH_ACR_LATENCY |
                    FLASH_ACR_PRFTEN |
                    FLASH_ACR_PRE_READ);
    // Enable the HSI oscillator, since the L0 series boots
    // to the MSI one.
    RCC->CR    |=  (RCC_CR_HSION);
    while (!(RCC->CR & RCC_CR_HSIRDY)) {};
    // Configure the PLL to use HSI16 with a PLLDIV of
    // 2 and PLLMUL of 4.
    RCC->CFGR  &= ~(RCC_CFGR_PLLDIV |
                    RCC_CFGR_PLLMUL |
                    RCC_CFGR_PLLSRC);
    RCC->CFGR  |=  (RCC_CFGR_PLLDIV2 |
                    RCC_CFGR_PLLMUL4 |
                    RCC_CFGR_PLLSRC_HSI);
    // Enable the PLL and wait for it to stabilize.
    RCC->CR    |=  (RCC_CR_PLLON);
    while (!(RCC->CR & RCC_CR_PLLRDY)) {};
    // Select the PLL as the system clock source.
    RCC->CFGR  &= ~(RCC_CFGR_SW);
    RCC->CFGR  |=  (RCC_CFGR_SW_PLL);
    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {};
    // Set the global clock speed variable.
    core_clock_hz = 32000000;
  #endif

  #ifdef VVC_F0
    // Enable the GPIOB peripheral in 'RCC_AHBENR'.
    RCC->AHBENR   |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR   |= RCC_AHBENR_GPIOBEN;
  #elif  VVC_L0
    RCC->IOPENR   |= RCC_IOPENR_IOPAEN;
    RCC->IOPENR   |= RCC_IOPENR_IOPBEN;
  #endif
  // Enable the SPI1 peripheral.
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

  #define VVC_HWSPI (1)
  //#define VVC_SWSPI (1)
  // Initialize the GPIOB pins.
  // Mode: Output
  GPIOB->MODER   &= ~((0x3 << (PB_MOSI * 2)) |
                      (0x3 << (PB_SCK  * 2)) |
                      (0x3 << (PB_DC   * 2)));
  #ifdef VVC_HWSPI
    // Set the MOSI and SCK pins to alternate function mode 0.
    // Set D/C to normal output.
    #ifdef VVC_F0
      GPIOB->AFR[0]  &= ~(GPIO_AFRL_AFSEL3 |
                          GPIO_AFRL_AFSEL5);
    #elif  VVC_L0
      GPIOB->AFR[0]  &= ~(GPIO_AFRL_AFRL3 |
                          GPIO_AFRL_AFRL5);
    #endif
    GPIOB->MODER   |=  ((0x2 << (PB_MOSI * 2)) |
                        (0x2 << (PB_SCK  * 2)) |
                        (0x1 << (PB_DC   * 2)));
    // Use pull-down resistors for the SPI peripheral?
    // Or no pulling resistors?
    GPIOB->PUPDR   &= ~((0x3 << (PB_MOSI * 2)) |
                        (0x3 << (PB_SCK  * 2)) |
                        (0x3 << (PB_DC   * 2)));
    GPIOB->PUPDR  |=   ((0x1 << (PB_MOSI * 2)) |
                        (0x1 << (PB_SCK  * 2)));
  #elif VVC_SWSPI
    // Set the pins to output mode.
    GPIOB->MODER   |=  ((0x1 << (PB_MOSI * 2)) |
                        (0x1 << (PB_SCK  * 2)) |
                        (0x1 << (PB_DC   * 2)));
    // No pull-up or pull-down resistors.
    GPIOB->PUPDR   &= ~((0x3 << (PB_MOSI * 2)) |
                        (0x3 << (PB_SCK  * 2)) |
                        (0x3 << (PB_DC   * 2)));
  #endif
  // Output type: Push-pull
  GPIOB->OTYPER  &= ~((0x1 << PB_MOSI) |
                      (0x1 << PB_SCK)  |
                      (0x1 << PB_DC));
  // High-speed - 50MHz maximum
  // (Setting all '1's, so no need to clear bits first.)
  GPIOB->OSPEEDR |=  ((0x3 << (PB_MOSI * 2)) |
                      (0x3 << (PB_SCK  * 2)) |
                      (0x3 << (PB_DC   * 2)));
  // Initialize the GPIOA pins; ditto.
  GPIOA->MODER   &= ~((0x3 << (PA_CS   * 2)) |
                      (0x3 << (PA_RST  * 2)));
  GPIOA->MODER   |=  ((0x1 << (PA_CS   * 2)) |
                      (0x1 << (PA_RST  * 2)));
  GPIOA->OTYPER  &= ~((0x1 << PA_CS) |
                      (0x1 << PA_RST));
  GPIOA->PUPDR   &= ~((0x3 << (PA_CS  * 2)) |
                      (0x3 << (PA_RST * 2)));
  // Perform ILI9341 panel initialization.
  // Set initial pin values.
  //   (The 'Chip Select' pin tells the display if it
  //    should be listening. '0' means 'hey, listen!', and
  //    '1' means 'ignore the SCK/MOSI/DC pins'.)
  GPIOA->ODR |=  (1 << PA_CS);
  //   (See the 'sspi_cmd' method for 'DC' pin info.)
  GPIOB->ODR |=  (1 << PB_DC);
  // Set SCK low to start.
  //GPIOB->ODR &= ~(1 << PB_SCK);
  // Or high?
  GPIOB->ODR |=  (1 << PB_SCK);
  // Reset the display by pulling the reset pin low,
  // delaying a bit, then pulling it high.
  GPIOA->ODR &= ~(1 << PA_RST);
  // Delay at least 100ms; meh, call it 2 million no-ops.
  delay_cycles(2000000);
  GPIOA->ODR |=  (1 << PA_RST);
  delay_cycles(2000000);

  // Send initialization commands for a 320x240 display.
  #ifdef VVC_HWSPI
    // Initialize the SPI peripheral.
    hspi_init(SPI1);
    // Pull CS low.
    GPIOA->ODR &= ~(1 << PA_CS);
    // Initialize the display.
    ili9341_hspi_init(SPI1);
  #elif  VVC_SWSPI
    ili9341_sspi_init();
  #endif

  // Main loop - empty the screen as a test.
  int tft_iter = 0;
  int tft_on = 0;
  #ifdef VVC_HWSPI
    // Set column range.
    hspi_cmd(SPI1, 0x2A);
    hspi_w16(SPI1, 0x0000);
    hspi_w16(SPI1, (uint16_t)(239));
    // Set row range.
    hspi_cmd(SPI1, 0x2B);
    hspi_w16(SPI1, 0x0000);
    hspi_w16(SPI1, (uint16_t)(319));
    // Set 'write to RAM'
    hspi_cmd(SPI1, 0x2C);
  #elif VVC_SWSPI
    // Set column range.
    sspi_cmd(0x2A);
    sspi_w(0x00);
    sspi_w(0x00);
    sspi_w(239 >> 8);
    sspi_w(239 & 0xFF);
    // Set row range.
    sspi_cmd(0x2B);
    sspi_w(0x00);
    sspi_w(0x00);
    sspi_w(319 >> 8);
    sspi_w(319 & 0xFF);
    // Set 'write to RAM'
    sspi_cmd(0x2C);
  #endif
  while (1) {
    #ifdef VVC_HWSPI
      // Write 320 * 240 pixels.
      for (tft_iter = 0; tft_iter < (320*240); ++tft_iter) {
        // Write a 16-bit color.
        if (tft_on) {
          hspi_w16(SPI1, 0xF800);
        }
        else {
          hspi_w16(SPI1, 0x001F);
        }
      }
      // Wait for the peripheral to finish sending.
      while ((SPI1->SR & SPI_SR_BSY)) {};
    #elif VVC_SWSPI
      // Write 320 * 240 pixels.
      for (tft_iter = 0; tft_iter < (320*240); ++tft_iter) {
        // Write a 16-bit color.
        if (tft_on) {
          sspi_w(0xF8);
          sspi_w(0x00);
        }
        else {
          sspi_w(0x00);
          sspi_w(0x1F);
        }
      }
    #endif

    tft_on = !tft_on;
  }
}
