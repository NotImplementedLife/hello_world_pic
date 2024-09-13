#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "picex_api.h"

typedef struct {
	int header;	
	void* exports;
	char binary[];
} picex_module;


picex_module* picex_load(const char* filename);
picex_module* picex_load_by_name(const char* module_name);
void picex_free(picex_module* module);

void picex_run_entrypoint(picex_module* module);


#ifdef __cplusplus
}
#endif