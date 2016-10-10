/*
    GNU linker script for STM32F412
*/

/* Specify the memory areas */
MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 0x010000 /* entire flash, 1 MiB */
    FLASH_ISR (rx)  : ORIGIN = 0x08000000, LENGTH = 0x004000 /* sector 0, 16 KiB */
                                                             /* sectors 1,2,3 are 16K, 4 is 64K (for filesystem) */
    FLASH_TEXT (rx) : ORIGIN = 0x08020000, LENGTH = 0x080000 /* sectors 5,6,7,8 4*128KiB = 512 KiB */
    RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 0x020000 /* 128 KiB */
}
 
/* produce a link error if there is not this amount of RAM for these sections */
_minimum_stack_size = 2K;
_minimum_heap_size = 16K;
 
/* top end of the stack */

/*_stack_end = ORIGIN(RAM) + LENGTH(RAM);*/
_estack = ORIGIN(RAM) + LENGTH(RAM);

/* define common sections and symbols */
INCLUDE common.ld

/* RAM extents for the garbage collector */
_ram_start = ORIGIN(RAM);
_ram_end = ORIGIN(RAM) + LENGTH(RAM);
_heap_start = _ebss; /* heap starts just after statically allocated memory */
_heap_end = 0x2001c000; /* tunable */
