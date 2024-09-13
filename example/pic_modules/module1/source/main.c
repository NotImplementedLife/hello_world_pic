short* BG0_SUB = ((short*)0x0620B002);

void print_chars(const char* message)
{    
    while(*message) *BG0_SUB++ = 0xF000|*message++;
}

typedef int (*printf_t)(const char* format, ...);

printf_t iprintf = 0;

void loadf(void* ptr) { iprintf = (printf_t)ptr; }

int sum(int a, int b) { return a + b; }

__attribute__((section(".text.entrypoint")))
void _start()
{	
	if(iprintf)
		iprintf("Hello world from PIC module!");
}

__attribute__((used))
const void* exports[] = {	
	loadf,
	sum
};
