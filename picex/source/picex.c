#include "picex.h"

#include <stdio.h>
#include <stdlib.h>

#define MOV_R7_R7 ".BYTE 0x3f, 0x46"
#define MOV_R6_R6 ".BYTE 0x36, 0x46"
#define MOV_R5_R5 ".BYTE 0x2D, 0x46"

#define DESMUME_LOG_CODE(MOV_X_X, message) asm volatile ( \
	"push {r6} \r\n\t" "mov r6, %0 \r\n\t" \
	MOV_X_X "\r\n\t" "pop {r6}\r\n\t" \
	:: "r"(message));

#define DEBUG

#ifdef DEBUG
	#define dbg_set_step(step, msg) step = (msg);
	#define dbg_log_step(step) { DESMUME_LOG_CODE(MOV_R7_R7, "An error occured"); DESMUME_LOG_CODE(MOV_R7_R7, step); }
#else
	#define dbg_set_step(step, msg) ;
	#define dbg_log_step(step) ;
#endif

typedef void (*proc_t)();

static const int PICEX_MAGIC = 0x78434950;
#define is_magic_valid(x) ((x)==PICEX_MAGIC)

picex_module* picex_load(const char* filename)
{
	const char* step = NULL;
	
	dbg_set_step(step, "picex: open");
	FILE* f = fopen(filename, "rb");    
	if(!f) 
		goto error;			
	
	dbg_set_step(step, "picex: check magic");
	int magic;		
	if(!(fread(&magic, sizeof(int), 1, f) && is_magic_valid(magic))) 		
	{
		printf("%x\n", magic);
		goto error;	
	}
	
	dbg_set_step(step, "picex: read header");
	int got_start, got_end, binary_length;
	if (!( fread(&got_start, sizeof(int), 1, f) 
		&& fread(&got_end, sizeof(int), 1, f)
		&& fread(&binary_length, sizeof(int), 1, f)))
		goto error;		
	
	dbg_set_step(step, "picex: bin alloc");	
	picex_module* module  = (picex_module*)malloc(sizeof(picex_module) + binary_length);
	if(!module) 
		goto error;
	    
	dbg_set_step(step, "picex: bin read");	
	if(!fread(module->binary, 1, binary_length, f))
		goto error;
		
    fclose(f);
	
	// fix got
	int* got_start_offset = (int*)((char*)module->binary + got_start);
	int* got_end_offset = (int*)((char*)module->binary + got_end);
	for(int* offset=got_start_offset; offset!=got_end_offset; ++offset)
		*offset += (int)module->binary;		
	
    return module;	
	
error:	
	dbg_log_step(step);
	fclose(f);
	return NULL;	
}

char picex_default_name[] = "nitro:/pix/";

picex_module* picex_load_by_name(const char* module_name)
{
	char filename[64];
	char* fn = filename;
	for(const char* c=picex_default_name;*c;*fn++=*c++);
	for(const char* c=module_name;*c;*fn++=*c++);
	*fn++='\0';
	return picex_load(filename);		
}

void picex_free(picex_module* module)
{
	free(module);
}

void picex_run_entrypoint(picex_module* module)
{
	int thumb_address = (int)module->binary + 1;
	proc_t thumb_proc = (proc_t)thumb_address;
	thumb_proc();
}