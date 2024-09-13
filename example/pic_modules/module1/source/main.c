#define MOV_R7_R7 ".BYTE 0x3f, 0x46"
#define MOV_R6_R6 ".BYTE 0x36, 0x46"
#define MOV_R5_R5 ".BYTE 0x2D, 0x46"	

#define DESMUME_LOG_CODE(MOV_X_X, message) asm volatile \
( \
	"push {r6} \r\n\t" \
	"mov r6, %0 \r\n\t" \
	MOV_X_X "\r\n\t" \
	"pop {r6}\r\n\t" \
	:: "r"(message) \
);

void logs(const char* msg) { DESMUME_LOG_CODE(MOV_R7_R7, msg); }
void logx(unsigned  a)
{	
	char* buf=0x020F0000;	
	char* s=buf+32;
	*--s='\0';
	for(int i=0;i<32;i+=4)
	{
		int d=(a>>i)&0xF;
		*--s = d<10 ? '0'+d : 'A'+d-10;
	}
	*--s='x';
	*--s='0';		
	logs(s);
}


short* BG0_SUB = ((short*)0x0620B000);

//#define BG0_SUB ((short*)0x0620B000)
void print_chars(const char* message)
{
    short* map_ptr = BG0_SUB;
    while(*message)
        *map_ptr++ = 0xF000|*message++;
}

__attribute__((section(".text.entrypoint")))
void _start()
{
	logs("Hello from PIC?");
	logx(0x12345678);
	logx((unsigned)BG0_SUB);
	logx((unsigned)&BG0_SUB);
	logx((unsigned)&BG0_SUB - (unsigned)&_start);
    print_chars("Hello world from PIC!");
}