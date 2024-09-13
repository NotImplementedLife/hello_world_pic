#include "picex.h"

#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

typedef void (*proc_t)();

static const int PICEX_MAGIC = 0x78434950;
#define is_magic_valid(x) ((x)==PICEX_MAGIC)

typedef struct
{
	int magic;
	int got_start, got_end;
	int exports_start, exports_end;
	int binary_length;
} picex_header;

picex_module* picex_load(const char* filename)
{
	const char* step = NULL;
	
	dbg_set_step(step, "picex: open");
	FILE* f = fopen(filename, "rb");    
	if(!f) 
		goto error;			
	
	dbg_set_step(step, "picex: read header");
	picex_header header;
	if(!(fread(&header, sizeof(header), 1, f)))
		goto error;	
		
	dbg_set_step(step, "picex: check magic");	
	if(!is_magic_valid(header.magic))
		goto error;
	
	dbg_set_step(step, "picex: bin alloc");	
	picex_module* module  = (picex_module*)malloc(sizeof(picex_module) + header.binary_length);
	if(!module) 
		goto error;
	    
	dbg_set_step(step, "picex: bin read");	
	if(!fread(module->binary, 1, header.binary_length, f))
		goto error;
		
    fclose(f);
	
	// fix got
	int* got_start_offset = (int*)((char*)module->binary + header.got_start);
	int* got_end_offset = (int*)((char*)module->binary + header.got_end);
	for(int* offset=got_start_offset; offset!=got_end_offset; ++offset)
		*offset += (int)module->binary;		
	
	// fix exports
	int* exports_start_offset = (int*)((char*)module->binary + header.exports_start);
	int* exports_end_offset = (int*)((char*)module->binary + header.exports_end);
	for(int* offset=exports_start_offset; offset!=exports_end_offset; ++offset)
		*offset += (int)module->binary;
	
	module->exports = exports_start_offset;	
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