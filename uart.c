/**
 * @file uart.c
 * @brief A bare-metal UART driver for a Cortex-R5F core.
 */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "uart.h"

// function declarations
void addlog(char* str);

// string to store the register values
char register_values[10];

#define Debug_MEM_LOG_SIZE 1024
extern __attribute__((section (".log_shared_mem"))) char gDebugMemLog[Debug_MEM_LOG_SIZE];

/**
 * @brief Initializes the UART driver.
 *
 * Configures the UART1 for 115200 baud rate, 8 data bits, no parity, and 1 stop bit.
 */
void uart1_init(void) { 


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
    addlog("LSR_REG after reset= ");
    sprintf(register_values,"%x\n",REG(LSR_REG));
    addlog(register_values);

    // 2. DISABLE UART MODE 
    // Writing 0x7 (Disable mode) ensures the module is static while we configure 
    // the baud rate registers.
    REG(MDR1_REG) = MDR1_DISABLE_MODE;                  
    addlog("2. Mode Disabling done!\n");                
    addlog("LSR_REG after reset= ");
    sprintf(register_values,"%x\n",REG(LSR_REG));
    addlog(register_values);

    // 3. BAUD RATE CONFIGURATION 

    // a. Switch to Register Access Mode A: Enable Divisor Latch Access (DIV_EN = 1)
    REG(LCR_REG) |= LCR_DIV_EN;
    addlog("LSR_REG after div en= ");
    sprintf(register_values,"%x\n",REG(LSR_REG));
    addlog(register_values);
    // b. Set the Divisor Latches (DLL/DLH)
    // For 115.2 kbps DLL is 0x1A and DLH is 0x00
    REG(RHR_THR_DLL_REG) = (uint32_t)(0x1A);         
    REG(IER_DLH_REG)     = (uint32_t)(0x00);         
    addlog("3. Buad Rate Configuration Done!\n");
    addlog("LSR_REG after reset= ");
    sprintf(register_values,"%x\n",REG(LSR_REG));
    addlog(register_values);

    // 4. PROTOCOL CONFIGURATION 
    REG(LCR_REG) = LCR_8N1; 
    addlog("4. Protocol Configuration done!\n");
    addlog("LSR_REG after reset= ");
    sprintf(register_values,"%x\n",REG(LSR_REG));
    addlog(register_values);

    // 5. Set UART_16X_MODE Mode
    REG(MDR1_REG) = UART_16X_MODE;
    addlog("5. Switched to UART_16X_MODE Mode!\n"); 
    addlog("LSR_REG after reset= ");
    sprintf(register_values,"%x\n",REG(LSR_REG));
    addlog(register_values);

    // 6. Switch to Operational Mode 
    REG(LCR_REG) &= ~LCR_DIV_EN;
    addlog("6. Switched to Operational Mode!\n");
    addlog("LSR_REG after reset= ");
    sprintf(register_values,"%x\n",REG(LSR_REG));
    addlog(register_values);
    addlog("UART Configuration Done!\n");
}

/**
 * @brief Transmits a single character via UART.
 * @param data The character to transmit.
 */
void uart1_putc(char data) {


    // 1. Wait for Transmit Holding Register Empty (THRE)
    // This check uses the Line Status Register (LSR) bit 5 (LSR_THRE)
    addlog("LSR_REG = ");
    sprintf(register_values,"%x\n",REG(LSR_REG));
    addlog(register_values);
    while (!(REG(LSR_REG) & LSR_THRE));
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


