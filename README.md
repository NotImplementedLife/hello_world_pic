# Position Independent Code on the DS

## How to build 

TO DO: adjust the process to compile it directly in Makefile

For Windows, open the `build_pic.bat` in the `pic/` directory and change the devkitARM path to the one on your system (I couldn't use the %DEVKITARM% environment variable because its path was relative to the msys2 stuff 
starting with "/opt/..."). Then, run the `build_pic.bat` file. You should see a file named `main.pic.bin` in `nitrofs/` directory. Once you complete that, just compile it normally with the `make` command.

main.pic.c
```C
// Note that in this position indepent code we don't have
// access to functions that exist at runtime (like printf).
// Because of that, I hardcoded the process of printing
// characters to the default console for the purpose of
// this demo.

#define BG0_SUB ((short*)0x0620B000)
void print_chars(const char* message)
{
	short* map_ptr = BG0_SUB;
	while(*message)
		*map_ptr++ = 0xF000|*message++;
	// Each character having its ascii code 0xXY will be
	// converted to tile data 0xF0XY.
	// 0xF is the color white on the default console's palette.
	// there is a bijective corespondence between the ascii code 0xXY and
	// the glyph of the character existing as tile id 0xXY.
}

// this will be the function that is actually called
__attribute__((section(".text.entrypoint")))
void _start()
{
	print_chars("Hello world from PIC!");
}
```

main.cpp - the PIC loader
```C
int main(void) {		
	consoleDemoInit();
	nitroFSInit(NULL);
	
	// load the compiled position-independent binary main.pic.bin from nitroFS
	void* my_external_code = load_position_independent_code("nitro:/main.pic.bin");
	// the start offset of my_external_code is the address of our
	// position independent function.
	// We get its address and cast it to pic_function callable function pointer.
	// Note the "+ 1" added to the offset. This tells the cpu to call this
	// function is THUMB mode (because it was compiled as thumb).
	pic_function my_external_function = (pic_function)((int)my_external_code + 1);	
	
	// call the function. It should print "Hello world from PIC!" on the bottom screen.
	my_external_function();	 
	
	// idle
	while(1) {swiWaitForVBlank();}
	return 0;
}
```

![image](https://github.com/user-attachments/assets/ffc852b7-a6a6-4417-a1d5-1407c6e6ada5)

