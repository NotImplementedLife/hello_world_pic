#include <nds.h>
#include <filesystem.h>
#include <stdio.h>
#include <picex.h> // custom PIC Executor library

// these functions are exported by the PIC module
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
		const module_interface* mi = (module_interface*)module->exports;
		// make iprintf visible to the module
		mi->loadf(iprintf);
		// call a module function
		int x = mi->sum(1,2);		
		iprintf("Result = %i\n", x);	
		
		// this will iprintf hello world...
		picex_run_entrypoint(module); 		
	}	
	
	while(1) swiWaitForVBlank();
	return 0;
}
