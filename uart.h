// function declarations
void uart1_init(void);
void uart1_putc(char);
void uart1_puts(const char *);


// Base address for the UART peripheral
#define UART1_BASE_ADDR 0x02810000

#define REG(offset) (*((volatile uint32_t *)(UART1_BASE_ADDR + offset)))

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
