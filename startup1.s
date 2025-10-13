@ ===================================
@ ARM Cortex-R5 Vector Table (startup.s)
@ ===================================

.section .vectors, "a", %progbits
.global _vector_table

_vector_table:
    B    Reset_Entry        @ 0x00: Reset
    B    Undefined_Handler  @ 0x04: Undefined Instruction
    B    SVC_Handler        @ 0x08: Supervisor Call
    B    Prefetch_Handler   @ 0x0C: Prefetch Abort
    B    Data_Handler       @ 0x10: Data Abort
    NOP                     @ 0x14: Reserved Vector
    B    IRQ_Handler        @ 0x18: Interrupt Request (IRQ)
    B    FIQ_Handler        @ 0x1C: Fast Interrupt Request (FIQ)

@ The labels (Reset_Handler, IRQ_Handler, etc.) are declared 
@ as external symbols, which the linker will resolve.
.extern _reset_handler
.extern Undefined_Handler
.extern SVC_Handler
.extern Prefetch_Handler
.extern Data_Handler
.extern IRQ_Handler
.extern FIQ_Handler

.section .text
.global Reset_Entry
Reset_Entry:
    /* Set the Stack Pointer (SP) using the symbol from the linker script */
    LDR SP, =__stack_end__
    
    /* Branch to the C-language reset handler function */
    B _reset_handler

    