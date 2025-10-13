/*******************************************************************************
* @file : linker_script.ld
*
* @brief : Linker script for the AM67A Main Domain R5F core.
*
* This script assumes the program will be loaded and executed from TCM.
******************************************************************************/


/* output format is 32 bit elf little endian arm                              */
OUTPUT_FORMAT("elf32-littlearm")                                                      

                                             

/* Start of the Heap                                                          */
/* uint32_t __heap_start__;                                                   */

/* Describing memory regions                                                  */
MEMORY                                                                         
{
                                  
    /* ATCM started at 0x0 address with size 32KB                             */
    ATCM ( rwx ) : ORIGIN = (0), LENGTH = (32K)                                  
                   
    /* BTCM started at 0x41010000 with size 32KB                              */
    BTCM ( rwx ) : ORIGIN = (0x41010000), LENGTH = (32K)                         
                   
    /* shared DRAM started at 0xA1200000 with shared size 14MB                */
    DRAM ( rwx ) : ORIGIN = (0xA1200000), LENGTH = (14M)                   
    
}

/* Setting Entry Point to _vector_table                                      */
ENTRY("_vector_table")                                                    

SECTIONS 
{
    .vectors :
        {
            . = ALIGN(4);
            KEEP(*(.vectors))
        } > ATCM
    .resource_table : 
        {
            . = ALIGN(4); 
            /* Ensure the magic number is the first thing in the section */
            /* This is the RALA magic number 0x4F4C4152 */
            LONG(0x4F4C4152) 
            
            /* Place the rest of the resource table structure immediately after the magic */
            *(.resource_table)
        } > DRAM    
    .text :
        {
            . = ALIGN(4);
            *(.text) 
            *(.text.*)
            . = ALIGN(4);
        } > ATCM
    .rodata :
        {
            . = ALIGN(4);
            *(.rodata) 
            *(.rodata.*)
            . = ALIGN(4);
        } > BTCM AT > ATCM
    .data :
        {
            . = ALIGN(4);
            __data_load_start__ = LOADADDR(.data);
            __data_start__ = . ;
            *(.data) 
            *(.data.*)
            . = ALIGN(4);
            __data_end__ = . ;
        } > BTCM AT > ATCM
    .bss :
        {
            . = ALIGN(4);
            __bss_start__ = . ;
            *(.bss) 
            *(.bss.*)
            . = ALIGN(4);
            __bss_end__ = . ;
        } > BTCM

    
    /* assigning the address after all these sections to the heap            */
    __heap_start__ = . ;

    /* assigning the last address to the stack                               */
    __stack_end__ = ORIGIN(ATCM) + LENGTH(ATCM);
}

