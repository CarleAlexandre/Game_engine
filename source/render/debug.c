#include "render_impl.h"

#define MAX_HISTORY 1000

static struct {
	uint8_t		chunks_buffer;//current states of chunk buffer
	uint32_t	chunks_draw;//number of drawn chunks
	uint32_t	chunks_load;//number of loaded chunks
}	debug_struct;

void	render_debug_draw(void) {
	static float	frame_history[MAX_HISTORY] = {0};
	static uint8_t	offset = 0;

	frame_history[offset] = GetFrameTime() * 1000;
	offset = (offset + 1) % MAX_HISTORY;
	
	float max_time = 0.01f;
	for (int i = 0; i < MAX_HISTORY; i++) {
		if (frame_history[i] > max_time) {
			max_time = frame_history[i];
		}
	}

	for (int i = 0; i < MAX_HISTORY; i++) {
		const float val = frame_history[i] / max_time;

		if (val > 16.7f) {
			DrawLine(10 + 2 * i, 110, 10 + 2 * i, 110 - 50 * val, RED);
		} else if (val > 4.8f) {
			DrawLine(10 + 2 * i, 110, 10 + 2 * i, 110 - 50 * val, WHITE);
		} else {
			DrawLine(10 + 2 * i, 110, 10 + 2 * i, 110 - 50 * val, GREEN);
		}
	}
}