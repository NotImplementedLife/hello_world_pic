#include <nds.h>
#include <filesystem.h>
#include <stdio.h>
#include <picex.h>

struct module_interface
{
	void (*loadf)(int (*)(const char*, ...));
	int (*sum)(int, int);	
};

int main(void) {		
	consoleDemoInit();
	nitroFSInit(NULL);	
	
	picex_module* module = picex_load_by_name("module1");	
	if(!module) {
		iprintf("Bad module\n");
	}
	else {
		const module_interface* mi = (const module_interface*)module->exports;
		mi->loadf(iprintf);		
		int x = mi->sum(1,2);		
		iprintf("%i\n", x);		
		picex_run_entrypoint(module);						
	}	
	
	while(1) swiWaitForVBlank();
	return 0;
}
