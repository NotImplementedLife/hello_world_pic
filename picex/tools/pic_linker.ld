SECTIONS
{
  . = 0x0000;  
  .text : { *(.text.entrypoint) *(.text*) }
  . = ALIGN(4);
  .rodata : { *(.rodata) }  
  . = ALIGN(4);
  .data : { *(.data) }
  . = ALIGN(4);
  .bss : { *(.bss) }
  . = ALIGN(4);
  .got : { *(.got) *(.got.plt) }  
}