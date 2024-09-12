SECTIONS
{
  . = 0x0000;
  .text : { *(.text.entrypoint) *(.text*) }  
  .rodata : { *(.rodata) }
  .data : { *(.data) }
  .bss : { *(.bss) }
}