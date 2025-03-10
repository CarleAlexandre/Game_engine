#include "interface/haven_core.h"

int main(int ac, char *av[]) {
	haven_thread_mgr_init();
	
	haven_thread_mgr_close();
	return(0);
}