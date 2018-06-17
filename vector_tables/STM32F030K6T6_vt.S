#ifndef _VVC_VECT_TBL
#define _VVC_VECT_TBL

.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

// Global values.
.global vtable
.global default_interrupt_handler

/*
 * Define the core vector table which will live at 0x00000000.
 * This will depend on the specific chip you are using, but
 * some things like the end-of-stack address, reset handler,
 * Non-Maskable Interrupt handler, HardFault handler, etc.,
 * are unlikely to change.
 * Still, for peripheral stuff, refer to your datasheet. Your
 * chip's vendor may also provide a template startup file,
 * which will probably have a 'complete' table you can reference
 * for that specific chip. This one is for an STM32F030[F4].
 * (The labels are just fancy names for memory addresses to jump
 *  to when a given event triggers.)
 */
.type vtable, %object
.section .vector_table,"a",%progbits
vtable:
    // 0-15
    .word _estack
    .word reset_handler
    .word NMI_handler
    .word hard_fault_handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word SVC_handler
    .word 0
    .word 0
    .word pending_SV_handler
    .word SysTick_handler
    // 16-31
    .word window_watchdog_IRQ_handler
    .word 0
    .word RTC_IRQ_handler
    .word flash_IRQ_handler
    .word RCC_IRQ_handler
    .word EXTI0_1_IRQ_handler
    .word EXTI2_3_IRQ_handler
    .word EXTI4_15_IRQ_handler
    .word 0
    .word DMA1_chan1_IRQ_handler
    .word DMA1_chan2_3_IRQ_handler
    .word DMA1_chan4_5_IRQ_handler
    .word ADC1_IRQ_handler
    .word TIM1_break_IRQ_handler
    .word TIM1_CC_IRQ_handler
    // 32-47
    .word 0
    .word TIM3_IRQ_handler
    .word 0
    .word 0
    .word TIM14_IRQ_handler
    .word TIM15_IRQ_handler
    .word TIM16_IRQ_handler
    .word TIM17_IRQ_handler
    .word I2C1_IRQ_handler
    .word I2C2_IRQ_handler
    .word SPI1_IRQ_handler
    .word SPI2_IRQ_handler
    .word USART1_IRQ_handler
    .word USART2_IRQ_handler
    .word 0
    .word 0
    // 48-49
    .word 0
    // (Location to boot from for RAM startup)
    #define boot_ram_base  0xF108F85F
    .word boot_ram_base

    /*
     * Setup weak aliases for each exception handler to the
     * default one. These can be updated later, or just
     * overridden since they're weak refs.
     * The reset_handler is set up separately.
     */
    .weak NMI_handler
    .thumb_set NMI_handler, default_interrupt_handler
    .weak hard_fault_handler
    .thumb_set hard_fault_handler, default_interrupt_handler
    .weak SVC_handler
    .thumb_set SVC_handler, default_interrupt_handler
    .weak pending_SV_handler
    .thumb_set pending_SV_handler, default_interrupt_handler
    .weak SysTick_handler
    .thumb_set SysTick_handler, default_interrupt_handler
    .weak window_watchdog_IRQ_handler
    .thumb_set window_watchdog_IRQ_handler, default_interrupt_handler
    .weak RTC_IRQ_handler
    .thumb_set RTC_IRQ_handler, default_interrupt_handler
    .weak flash_IRQ_handler
    .thumb_set flash_IRQ_handler, default_interrupt_handler
    .weak RCC_IRQ_handler
    .thumb_set RCC_IRQ_handler, default_interrupt_handler
    .weak EXTI0_1_IRQ_handler
    .thumb_set EXTI0_1_IRQ_handler, default_interrupt_handler
    .weak EXTI2_3_IRQ_handler
    .thumb_set EXTI2_3_IRQ_handler, default_interrupt_handler
    .weak EXTI4_15_IRQ_handler
    .thumb_set EXTI4_15_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan1_IRQ_handler
    .thumb_set DMA1_chan1_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan2_3_IRQ_handler
    .thumb_set DMA1_chan2_3_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan4_5_IRQ_handler
    .thumb_set DMA1_chan4_5_IRQ_handler, default_interrupt_handler
    .weak ADC1_IRQ_handler
    .thumb_set ADC1_IRQ_handler, default_interrupt_handler
    .weak TIM1_break_IRQ_handler
    .thumb_set TIM1_break_IRQ_handler, default_interrupt_handler
    .weak TIM1_CC_IRQ_handler
    .thumb_set TIM1_CC_IRQ_handler, default_interrupt_handler
    .weak TIM3_IRQ_handler
    .thumb_set TIM3_IRQ_handler, default_interrupt_handler
    .weak TIM14_IRQ_handler
    .thumb_set TIM14_IRQ_handler, default_interrupt_handler
    .weak TIM15_IRQ_handler
    .thumb_set TIM15_IRQ_handler, default_interrupt_handler
    .weak TIM16_IRQ_handler
    .thumb_set TIM16_IRQ_handler, default_interrupt_handler
    .weak TIM17_IRQ_handler
    .thumb_set TIM17_IRQ_handler, default_interrupt_handler
    .weak I2C1_IRQ_handler
    .thumb_set I2C1_IRQ_handler, default_interrupt_handler
    .weak I2C2_IRQ_handler
    .thumb_set I2C2_IRQ_handler, default_interrupt_handler
    .weak SPI1_IRQ_handler
    .thumb_set SPI1_IRQ_handler, default_interrupt_handler
    .weak SPI2_IRQ_handler
    .thumb_set SPI2_IRQ_handler, default_interrupt_handler
    .weak USART1_IRQ_handler
    .thumb_set USART1_IRQ_handler, default_interrupt_handler
    .weak USART2_IRQ_handler
    .thumb_set USART2_IRQ_handler, default_interrupt_handler
.size vtable, .-vtable

/*
 * A 'Default' interrupt handler. This is where interrupts
 * which are not otherwise configured will go.
 * It is an infinite loop, because...well, clearly we weren't
 * expecting the interrupt, so what can we do?
 */
.section .text.default_interrupt_handler,"ax",%progbits
default_interrupt_handler:
    default_interrupt_loop:
      B default_interrupt_loop
.size default_interrupt_handler, .-default_interrupt_handler

#endif
