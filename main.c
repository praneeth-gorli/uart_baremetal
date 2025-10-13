/**
 * @brief The main entry point for the bare-metal application.
 */

// function declarations
void uart1_init(void);
void uart1_putc(char);
char uart1_getc(void);
void uart1_puts(const char *);


int main(void) {
    uart1_init();
    uart1_puts("Hello World\n");

    while (1) {
        // Echo the received character
        char c = uart1_getc();
        uart1_putc(c); 
    }

    return 0;
}