#define BG0_SUB ((short*)0x0620B000)
void print_chars(const char* message)
{
    short* map_ptr = BG0_SUB;
    while(*message)
        *map_ptr++ = 0xF000|*message++;
}

__attribute__((section(".text.entrypoint")))
void _start()
{
    print_chars("Hello world from PIC!");
}