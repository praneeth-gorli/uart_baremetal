/**
 * @file uart.c
 * @brief A bare-metal UART driver for a Cortex-R5F core.
 */
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// string to store the register values
char register_values[10];

#define DebugP_MEM_LOG_SIZE 1024
extern __attribute__((section (".log_shared_mem"))) char gDebugMemLog[DebugP_MEM_LOG_SIZE];

#define REG(offset) (*((volatile uint32_t *)(UART1_BASE_ADDR + offset)))

#define CTRL_MMR0_CFG0_USART1_CLKSEL  (*(uint32_t *)0x00108284)
#define CTRL_MMR0_CFG0_USART1_CLKSEL_PROXY (*(uint32_t *)0x0010A284)


// Base address for the UART peripheral (Placeholder value, replace with actual address)
#define UART1_BASE_ADDR 0x02810000

// Register Offsets 
#define RHR_THR_DLL_REG 0x00     // R: RHR, W: THR, DLL when LCR[7]=1
#define IER_DLH_REG     0x04     // R: IER, W: IER, DLH when LCR[7]=1
#define IIR_FCR_REG     0x08     // R: IIR, W: FCR
#define LCR_REG         0x0C     // Line Control Register
#define LSR_REG         0x14     // Line Status Register (Used for TX status)

// TI-specific control registers mentioned in the document
#define SYSC_REG        0x54     // System Configuration (for software reset)
#define SYSS_REG        0x58     // System Status (for reset done check)
#define MDR1_REG        0x20     // Mode Definition Register 1 (for operational mode)


// --- BIT FIELD DEFINITIONS ---
// LCR Register Bits (LCR_REG, offset 0x0C)
#define LCR_DIV_EN (1 << 7)     // Divisor Latch Access Enable (Mode A)
#define LCR_8N1    0x03         // 8-bit data, No parity, 1 stop bit 

// LSR Register Bits (LSR_REG, offset 0x14)
#define LSR_THRE   (1 << 5)     // Transmit Holding Register Empty

// FCR Register Bits (IIR_FCR_REG, offset 0x08)
#define FCR_FIFO_EN (1 << 0)    // FIFO Enable
#define FCR_RX_CLR  (1 << 1)    // RX FIFO Reset
#define FCR_TX_CLR  (1 << 2)    // TX FIFO Reset

// SYSC Register Bits (SYSC_REG, offset 0x54)
#define SYSC_SOFTRESET (1 << 1) // Software Reset

// SYSS Register Bits (SYSS_REG, offset 0x58)
#define SYSS_RESETDONE (1 << 0) // Reset Done

// MDR1 Register Bits (MDR1_REG, offset 0x20)
#define MDR1_DISABLE_MODE 0x7 // Disable mode as per Table 12-94
#define UART_16X_MODE 0X0     // UART 16x mode


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
void uart1_puts(const char *);
void addlog(char* str);

/**
 * @brief Initializes the UART driver.
 *
 * Configures the UART1 for 115200 baud rate, 8 data bits, no parity, and 1 stop bit.
 */
void uart1_init(void) {

    CTRL_MMR0_CFG0_USART1_CLKSEL = 0x1;  
    CTRL_MMR0_CFG0_USART1_CLKSEL_PROXY =0x1;


    // pad configuration
    // pad configuration for RX
    PADCONFIG107 &=  ~(0x1<<31);   // Register UnLocked
    PADCONFIG107 &=  ~(0x1<<27);   // Deep sleep Pull up Pull Down enabled
    PADCONFIG107 &=  ~(0x1<<21);   // Driver Enabled
    PADCONFIG107 |=   (0x1<<18);   // Receiver Enable 
    PADCONFIG107 |=   (0x1<<16);   // Pull Disable
    PADCONFIG107 &=  ~(0x1<<14);   // Schmitt Trigger Disable
    PADCONFIG107 &= ~ (0xF);       // Clear 3:0 Bits
    PADCONFIG107 |=   (0x2);       // Changing MUX mode to 2

    // pad configuration for TX
    PADCONFIG108 &=  ~(0x1<<31);   // Register UnLocked
    PADCONFIG108 &=  ~(0x1<<27);   // Deep sleep Pull up Pull Down enabled
    PADCONFIG108 &=  ~(0x1<<21);   // Driver Enabled
    PADCONFIG108 &=  ~(0x1<<14);   // Schmitt Trigger Disable
    PADCONFIG108 |=   (0x1<<16);   // Pull Disable
    PADCONFIG108 &=  ~(0x1<<18);   // Receiver Disable 
    PADCONFIG108 &=  ~(0xF);       // Clear 3:0 Bits
    PADCONFIG108 |=   (0x2);       // Changing MUX mode to 2
    addlog("PAD configuration done!\n");


    // 1. Perform Software Reset 
    
    // Set SOFTRESET bit
    REG(SYSC_REG) |= SYSC_SOFTRESET; 
    addlog("Initiated Reset\n");

    // Wait until RESETDONE is set to 1
    while (!(REG(SYSS_REG) & SYSS_RESETDONE));
    addlog("1. Reset done!\n");

    // 2. DISABLE UART MODE 
    // Writing 0x7 (Disable mode) ensures the module is static while we configure 
    // the baud rate registers.
    REG(MDR1_REG) = MDR1_DISABLE_MODE;                  
    addlog("2. Mode Disabling done!\n");                

    // 3. BAUD RATE CONFIGURATION 

    // a. Switch to Register Access Mode A: Enable Divisor Latch Access (DIV_EN = 1)
    REG(LCR_REG) |= LCR_DIV_EN;

    // b. Set the Divisor Latches (DLL/DLH)
    // For 115.2 kbps DLL is 0x1A and DLH is 0x00
    REG(RHR_THR_DLL_REG) = (uint32_t)(0x1A);         
    REG(IER_DLH_REG)     = (uint32_t)(0x00);         
    addlog("3. Buad Rate Configuration Done!\n");

    // 4. PROTOCOL CONFIGURATION 
    REG(LCR_REG) = LCR_8N1; 
    addlog("4. Protocol Configuration done!\n");

    // 5. Set UART_16X_MODE Mode
    REG(MDR1_REG) = UART_16X_MODE;
    addlog("5. Switched to UART_16X_MODE Mode!\n"); 
    
    // 6. Switch to Operational Mode 
    REG(LCR_REG) &= ~LCR_DIV_EN;
    addlog("6. Switched to Operational Mode!\n");
    
    addlog("UART Configuration Done!\n");
    /*addlog("UART1_MDR1 = ");
    sprintf(register_values,"%x\n",REG(MDR1_REG));
    addlog(register_values);*/
}

/**
 * @brief Transmits a single character via UART.
 * @param data The character to transmit.
 */
void uart1_putc(char data) {


    // 1. Wait for Transmit Holding Register Empty (THRE)
    // This check uses the Line Status Register (LSR) bit 5 (LSR_THRE)
    //while (!(REG(LSR_REG) & LSR_THRE));
    if(!(REG(LSR_REG) & LSR_THRE))
    {
        addlog("Transmit Register Not Empty\n");
    }
    addlog("UART TRX is empty\n");

    // 2. Write the data to the Transmit Holding Register (THR)
    // The THR shares the same address as the DLL/RHR, accessible when LCR[7]=0.
    REG(RHR_THR_DLL_REG) = (uint32_t)data;
    addlog("Written Data\n");
    addlog("RHR_THR_DLL_REG = ");
    sprintf(register_values,"%x\n",REG(RHR_THR_DLL_REG));
    addlog(register_values);

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


