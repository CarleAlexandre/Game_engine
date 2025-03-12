#include "interface/haven_core.h"

// #define CR_HOST
// #include <extern/cr.h>

void	hot_reload_game() {

}

int main(int ac, char *av[]) {
	haven_memory_system_init();
	haven_thread_mgr_init();
	
	haven_thread_mgr_close();
	haven_memory_system_shutdown();
	return(0);
}
