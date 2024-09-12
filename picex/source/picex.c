#include "picex.h"

#include <stdio.h>
#include <stdlib.h>

typedef void (*proc_t)();

picex_module* picex_load(const char* filename)
{
	FILE* f = fopen(filename, "rb");    
	if(!f) return NULL;
	
    fseek(f, 0 , SEEK_END);
    int fileSize = ftell(f);
    fseek(f, 0 , SEEK_SET);	
	picex_module* module  = (picex_module*)malloc(sizeof(picex_module) + fileSize);		
	if(!module) return NULL;
	
	fread(module->binary, fileSize, 1, f);
    fclose(f);
    return module;	
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