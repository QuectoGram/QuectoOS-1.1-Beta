ENTRY(kernel_main)

SECTIONS
{
    . = 0x10200;

    .text : {
        *(.text.kernel_main)
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
        *(.rdata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
        *(COMMON)
    }

    /DISCARD/ : {
        *(.pdata)
        *(.xdata)
        *(.eh_frame)
        *(.comment)
        *(.idata*)
        *(.reloc*)
    }
}
