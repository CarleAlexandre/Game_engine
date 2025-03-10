#include <time.h>
#include <stdint.h>
#include <windows.h>

time_t	previous, current, start;

void	haven_time_usleep(uint64_t usec) {
	HANDLE timer; 
	LARGE_INTEGER ft; 
    
	ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time
    
	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}

void	haven_time_init() {
	current = time(0x00);
	previous = current;
	start = current;
}

void	haven_time_update() {
	previous = current;
	current = time(0x00);
}

double	haven_time_frame() {
	return (difftime(current, previous));
}

double	haven_time_elapsed() {
	return (difftime(time(0x00), start));
}
