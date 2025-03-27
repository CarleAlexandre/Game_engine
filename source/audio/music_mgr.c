#include <stdlib.h>
#include <assert.h>
#include <raylib.h>

static struct {
	unsigned int	size;
	Music		*music;
}	atlas = {0};

static Music	current = {0};

#define MAX_TIME_WITHOUT_MUSIC 10

unsigned int	haven_music_init(const char **music_files, unsigned int n_music) {
	atlas.music = malloc(sizeof(Music) * n_music);
	assert(atlas.music);

	for (int i = 0; i < n_music; i++) {
		Music	span = LoadMusicStream(music_files[i]);
		if (IsMusicValid(span)) {
			atlas.size++;
			atlas.music[i] = span;
		}
	}
	return (atlas.size);
}

void	haven_music_play(unsigned int idx) {
	if (IsMusicStreamPlaying(current)) {
		StopMusicStream(current);
	}
	current = atlas.music[idx];
	PlayMusicStream(current);
}

void	haven_music_update(void) {
	static double value = 0.0;

	if (!IsMusicStreamPlaying(current)) {
		value += GetFrameTime();
		if (value >= MAX_TIME_WITHOUT_MUSIC) {
			int rand = GetRandomValue(0, atlas.size - 1);
			current = atlas.music[rand];
			PlayMusicStream(current);
			value = 0.0;
		}
	}
	UpdateMusicStream(current);
}

void	haven_music_close(void) {
	for (int i = 0; i < atlas.size; i++) {
		UnloadMusicStream(atlas.music[i]);
	}
	CloseAudioDevice();
}
