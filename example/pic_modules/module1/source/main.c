
typedef int (*printf_t)(const char* format, ...);
// pointer to external printf function
// to be loaded later
printf_t iprintf = 0; 

void loadf(printf_t ptr) { iprintf = ptr; }
int sum(int a, int b) { return a + b; }

// default module execution path
__attribute__((section(".text.entrypoint")))
void _start()
{	
	if(iprintf)
		iprintf("Hello world from PIC module!");
}

// functions to export
__attribute__((used))
const void* exports[] = {	
	loadf,
	sum
};
