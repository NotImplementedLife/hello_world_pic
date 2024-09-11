#include <nds.h>
#include <stdio.h>
#include <fat.h>
#include <filesystem.h>

// we call our position independent code via
// this function pointer
typedef void(*pic_function)();

// create a buffer that will hold the instructions 
// loaded from file
void* load_position_independent_code(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	// load all contents of the file into memory
	fseek(f, 0 , SEEK_END);
	int fileSize = ftell(f);		
	fseek(f, 0 , SEEK_SET);
	char* binary = (char*)malloc(fileSize);		
	fread(binary, fileSize, 1, f);	
	fclose(f);
	return binary;
}
	
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
