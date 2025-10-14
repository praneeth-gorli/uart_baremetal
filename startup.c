/**
 * @file startup.c
 * @brief Startup code for the R5F core on the TI AM67A.
 *
 * The main tasks performed are:
 * 1. Defining the exception vector table.
 * 2. Handling the reset sequence, which includes:
 * - Clearing the uninitialized BSS section in RAM.
 * 3. Calling the main() function to start the application.
 */

#include <stdint.h>

// External symbols defined in the linker script.
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __stack_end__;

// Function Prototypes
extern int main(void);
void _reset_handler(void);
void _default_handler(void);
// Alias other handlers to the default handler
void Undefined_Handler(void) __attribute__((weak, alias("_default_handler")));
void SVC_Handler(void)       __attribute__((weak, alias("_default_handler")));
void Prefetch_Handler(void)  __attribute__((weak, alias("_default_handler")));
void Data_Handler(void)      __attribute__((weak, alias("_default_handler")));
void IRQ_Handler(void)       __attribute__((weak, alias("_default_handler")));
void FIQ_Handler(void)       __attribute__((weak, alias("_default_handler")));


/* Reset Handler
 * This is the first C function executed after a reset. It prepares the memory
 * for the application code and then transfers control to main().
 */
void _reset_handler(void) {
    uint32_t * destination;

    
    // Zero-fill the BSS section (uninitialized data)
    destination = &__bss_start__;
    while (destination < &__bss_end__) {
        *destination++ = 0;
    }

    // Transfer control to the main 
    main();

    // superloop after main execution completed
    while (1);
}

/*
 * Default Exception Handler
 * This handler is called for any unhandled exceptions or interrupts.
 */
void _default_handler(void) {
    while (1);
}



