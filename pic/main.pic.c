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
