:: your devkitARM path here
@set devkitarm=D:\Software\Developer\devkitPro\devkitARM

@set gcc=%devkitarm%\bin\arm-none-eabi-gcc.exe
@set objcopy=%devkitarm%\arm-none-eabi\bin\objcopy.exe
@set objdump=%devkitarm%\arm-none-eabi\bin\objdump.exe

@set flags=-O2 -mthumb -march=armv5te -mtune=arm946e-s -fpic -ffreestanding -nostdlib -nostartfiles -Wl,--section-start=.text=0x00000000,-Tpic_linker.ld

%gcc% main.pic.c %flags% -o main.pic.o
%objdump% -D main.pic.o
%objcopy% -O binary main.pic.o ../nitro/main.pic.bin
hexdump -C ../nitro/main.pic.bin