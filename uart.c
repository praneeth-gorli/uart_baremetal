/**
 * @file uart.c
 * @brief A bare-metal UART driver for a Cortex-R5F core.
 */
#include <stdint.h>
// Main Domain UART1 Base Address 
#define UART1_BASE_ADDR 0x02810000

// UART Register Offsets
#define UART1_THR  (*(volatile uint8_t *)(UART1_BASE_ADDR+0x00))  // Transmit Holding Register
#define UART1_DLL  (*(uint8_t *)(UART1_BASE_ADDR+0x00))           // Divisor Latch Low
#define UART1_DLH  (*(uint8_t *)(UART1_BASE_ADDR+0x04))           // Divisor Latch High
#define UART1_LCR  (*(uint32_t *)(UART1_BASE_ADDR+0x0C))          // Line Control Register
#define UART1_MDR1 (*(uint32_t *)(UART1_BASE_ADDR+0x20))          // Mode Definition Register
#define UART1_LSR  (*(volatile uint32_t *)(UART1_BASE_ADDR+0x14)) // Line Status Register
#define UART1_RBR  (*(volatile uint8_t *)(UART1_BASE_ADDR+0x00))  // Receive Buffer Register
// Register Bit Masks
#define UART_LCR_DIV_EN    (1 << 7) // Divisor Latch Enable
#define UART_LSR_TX_FIFO_E (1 << 5) // 0-Transmitter Register Full, 1-Transmitter Register Empty
#define UART_LSR_RX_FIFO_E (1 << 0) // 0-Receive Buffer Empty, 1-Receive Buffer Full

// PADCONFIG Registers
#define PADCFG_CTRL0_CFG0 0x000F0000
// UART1 RX Pin No : C27, Ball Name : MCASP0_AFSR
// PadConfig Register PADCONFIG107
#define PADCONFIG107 (*(int *)(PADCFG_CTRL0_CFG0+0x41AC))
// UART1 TX Pin No : F24, Ball Name : MCASP0_ACLKR
// PadConfig Register PADCONFIG108
#define PADCONFIG108 (*(int *)(PADCFG_CTRL0_CFG0+0x41B0))


// function declarations
void uart1_init(void);
void uart1_putc(char);
char uart1_getc(void);
void uart1_puts(const char *);

/**
 * @brief Initializes the UART driver.
 *
 * Configures the UART1 for 115200 baud rate, 8 data bits, no parity, and 1 stop bit.
 */
void uart1_init(void) {


    // pad configuration
    // pad configuration for RX
    PADCONFIG107 &= ~(0xF);       // Clear 3:0 Bits
    PADCONFIG107 |=  (0x2);       // Changing MUX mode to 2
    PADCONFIG107 |=  (0x1<<14);   // Schmitt Trigger Enable
    PADCONFIG107 |=  0x1<<16;     // Pull Disable
    PADCONFIG107 |=  (0x1<<18);   // Receiver Enable 
    PADCONFIG107 |=  (0x1<<31);   // Register Locked
    // pad configuration for TX
    PADCONFIG108 &= ~(0xF);       // Clear 3:0 Bits
    PADCONFIG108 |=  (0x2);       // Changing MUX mode to 2
    PADCONFIG108 |=  (0x1<<14);   // Schmitt Trigger Enable
    PADCONFIG108 |=  0x1<<16;     // Pull Disable
    PADCONFIG108 &= ~(0x1<<18);   // Receiver Disable 
    PADCONFIG108 |=  (0x1<<31);   // Register Locked


    // 1. Configuring the MODE to UART16x (<= 230.4 kbps)
    UART1_MDR1 &= ~(0X7);

    // 2. Configure the Divisor Latch Enable to enable access to divisor registers
    UART1_LCR |= UART_LCR_DIV_EN;

    // 3. Set the baud rate to 115200 (for a 48 MHz clock, divisor is 26)
    // Formula: Divisor = Clock_Rate / (16 * Baud_Rate)
    UART1_DLL = 26;
    UART1_DLH = 0;

    // 4. Disable Divisor Latch Disable
    UART1_LCR &= ~UART_LCR_DIV_EN;

    // 5. set line settings: 8 data bits, 1 stop bit, no parity
    UART1_LCR = 0x03;

}

/**
 * @brief Transmits a single character via UART.
 * @param data The character to transmit.
 */
void uart1_putc(char data) {
    // Wait until the Transmit Holding Register is empty
    while (!(UART1_LSR & UART_LSR_TX_FIFO_E));

    // Write the data to the Transmit Holding Register
    UART1_THR = data;
}

/**
 * @brief Receives a single character from the UART.
 * @return The received character.
 */
char uart1_getc(void) {
    // Wait until data is ready in the Receive Buffer
    while (!(UART1_LSR & UART_LSR_RX_FIFO_E));

    // Read the data from the Receive Buffer Register
    return (char)(UART1_RBR);
}

/**
 * @brief Prints a string via UART.
 * @param str The string to print.
 */
void uart1_puts(const char *str) {
    while (*str) {
        uart1_putc(*str++);
    }
}


