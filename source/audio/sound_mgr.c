#include <stdlib.h>
#include <assert.h>
#include <raylib.h>

static struct {	
	unsigned int	size;
	Sound		*sound;
}	atlas;


/*
sound queue
*/

#define MAX_CONCURENT_SOUND 10
struct {
	unsigned int	idx;
	Sound		data[MAX_CONCURENT_SOUND];
}	queue;

static void	sound_queue_add(Sound sound) {
	Sound toplay = LoadSoundAlias(sound);

	if (IsSoundValid(queue.data[queue.idx])) {
		UnloadSoundAlias(queue.data[queue.idx]);
	}
	PlaySound(toplay);
	queue.data[queue.idx] = toplay;
	queue.idx ++;
	queue.idx %= 10;
}

/*
	actual implementation
*/

bool	haven_sound_play(unsigned int idx) {
	if (!IsSoundValid(atlas.sound[idx])) {
		return (false);
	}
	sound_queue_add(atlas.sound[idx]);
	return (true);
}

unsigned int	haven_sound_init(const char **sound_files, unsigned int n_sound) {
	atlas.sound = malloc(sizeof(Sound) * n_sound);
	assert(atlas.sound);

	for (int i = 0; i < n_sound; i++) {
		Sound	tmp_sound = LoadSound(sound_files[i]);
		if (IsSoundValid(tmp_sound)) {
			atlas.size++;
			atlas.sound[i] = tmp_sound;
		}
	}
	queue.idx = 0;
	return (atlas.size);
}

void	haven_sound_close(void) {
	for (int i = 0; i < atlas.size; i++) {
		UnloadSound(atlas.sound[i]);
	}
	CloseAudioDevice();
}
