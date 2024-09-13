#include <nds.h>
#include <stdio.h>
#include <fat.h>
#include <filesystem.h>

#include <picex.h>

int main(void) {		
	consoleDemoInit();
	nitroFSInit(NULL);	
	
	picex_module* module = picex_load_by_name("module1");
	
	if(!module) {
		iprintf("Bad module\n");
	}
	else {
		picex_run_entrypoint(module);
	}
		
	while(1) swiWaitForVBlank();
	return 0;
}
