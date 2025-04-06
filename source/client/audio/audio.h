#ifndef HAVEN_SOUND_H
# define HAVEN_SOUND_H

#include <stdbool.h>

/*
	SOUND
*/

/**
 * @brief add sound to play
 * 
 * @param idx 
 * @return true 
 * @return false 
 */
bool	haven_sound_play(unsigned int idx);

/**
 * @brief initialize audio device and load sound atlas
 * 
 * @param sound 
 * @param n_sound 
 * @return unsigned int 
 */
unsigned int	haven_sound_init(const char **sound, unsigned int n_sound);

/**
 * @brief close audio device and unload sound atlas;
 * 
 */
void	haven_sound_close(void);


/*
	MUSIC
*/

/**
 * @brief 
 * 
 * @param idx 
 */
void	haven_music_play(unsigned int idx);

/**
 * @brief 
 * 
 */
void	haven_music_update(void);

/**
 * @brief 
 * 
 * @param music_files 
 * @param n_music 
 * @return unsigned int 
 */
unsigned int	haven_music_init(const char **music_files, unsigned int n_music);

/**
 * @brief 
 * 
 */
void	haven_music_close(void);


#endif