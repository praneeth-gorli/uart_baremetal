/*******************************************************************************
* @file : linker_script.ld
*
* @brief : Linker script for the AM67A Main Domain R5F core.
*******************************************************************************/


/* refer this for linker script https://mcyoung.xyz/2021/06/01/linker-script/ */

/* output format is 32 bit elf little endian arm                              */
OUTPUT_FORMAT("elf32-littlearm")                                                      

                                             

/* Start of the Heap                                                          */
/* uint32_t __heap_start__;                                                   */

/* Describing memory regions                                                  */
MEMORY                                                                         
{
                             
    /* ATCM started at 0x0 address with size 32KB                             */
    ATCM ( rwx ) : ORIGIN = (0x0), LENGTH = (32K)                                  
                   
    /* BTCM started at 0x41010000 with size 32KB                              */
    BTCM ( rwx ) : ORIGIN = (0x41010000), LENGTH = (32K)                         
                   
    /* shared DRAM started at 0xA1200000 with shared size 14MB                */
    DRAM ( rwx ) : ORIGIN = (0x00A2000000), LENGTH = (14M)          

    /* Resource Table Memory Section                                          */
    RSC_TABLE ( rw ) : ORIGIN = (0xA2100000), LENGTH = (0x100000)         
    
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
    .rproc_resource_table_sect (NOLOAD) :
        {
            KEEP(*(.resource_table))
        } > RSC_TABLE    
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
        } > ATCM
    .data :
        {
            . = ALIGN(4);
            __data_start__ = . ;
            *(.data) 
            *(.data.*)
            . = ALIGN(4);
            __data_end__ = . ;
        } > ATCM
    .bss :
        {
            . = ALIGN(4);
            __bss_start__ = . ;
            *(.bss) 
            *(.bss.*)
            . = ALIGN(4);
            __bss_end__ = . ;
        } > ATCM

    
    /* assigning the address after all these sections to the heap            */
    __heap_start__ = . ;

    /* assigning the last address to the stack                               */
    __stack_end__ = ORIGIN(ATCM) + LENGTH(ATCM);
}

