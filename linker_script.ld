/*******************************************************************************
* @file : linker_script.ld
*
* @brief : Linker script for the AM67A Main Domain R5F core.
*
* This script assumes the program will be loaded and executed from TCM.
******************************************************************************/

MEMORY
{
  ATCM (rwx) : ORIGIN = 0, LENGTH = 0x8000
  BTCM ( rwx ) : ORIGIN = (0x41010000), LENGTH = (32K)
  DDR (rwx) : ORIGIN = 0xA2200000, LENGTH = (14M)
  RSC_TABLE ( rw ) : ORIGIN = (0xA2100000), LENGTH = (0x100000)
}

SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vector))
		*(.text*)

		KEEP(*(.init))
		KEEP(*(.fini))

		*(.rodata*)
		KEEP(*(.eh_frame*))
	} > ATCM

	.ARM.extab : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > ATCM

	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > ATCM
	__exidx_end = .;


	/* Location counter can end up 2byte aligned with narrow Thumb code but
	   __etext is assumed by startup code to be the LMA of a section in RAM
	   which must be 4byte aligned */
	__etext = ALIGN (4);

	.data : AT (__etext)
	{
		__data_start__ = .;
		*(.data*)
		. = ALIGN(4);
		__data_end__ = .;

	} > ATCM

	.bss :
	{
		. = ALIGN(4);
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
	} > ATCM
	
	.heap (COPY):
	{
		__end__ = .;
		PROVIDE(end = .);
		*(.heap*)
		__HeapLimit = .;
	} > ATCM

	/* .stack_dummy section doesn't contains any symbols. It is only
	 * used for linker to calculate size of stack sections, and assign
	 * values to stack symbols later */
	.stack_dummy (COPY):
	{
		*(.stack*)
	} > ATCM

	/* Set stack top to end of RAM, and stack limit move down by
	 * size of stack_dummy section */
	__StackTop = ORIGIN(ATCM) + LENGTH(ATCM);
	__StackLimit = __StackTop - SIZEOF(.stack_dummy);
	PROVIDE(__stack = __StackTop);
	
	/* Check if data + heap + stack exceeds RAM limit */
	ASSERT(__StackLimit >= __HeapLimit, "region ATCM overflowed with stack")

	.resource_table :
	{
		*(.resource_table*)
	} > RSC_TABLE

	.log_shared_mem :
	{
		*(.log_shared_mem*)
	} > DDR
}
