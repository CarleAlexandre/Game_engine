#ifndef GUI_H
#define GUI_H

#include <stdbool.h>
#include <raylib.h>

typedef enum {
	BUTTON_DEFAULT = 0,
	BUTTON_DOWN = 1,
	BUTTON_RELEASE = 2,
	BUTTON_HOVER = 3,
}	button_state;

typedef struct {
	button_state	state;
	bool		collision;
	char		*str;
	Rectangle	bound;
	int		texture_id;
}	haven_gui_button;

/**
 * @brief create a new button
 * 
 * @param text 
 * @param bound 
 * @param texture_id 
 * @return haven_gui_button* 
 */
haven_gui_button	*haven_gui_button_create(const char *text, Rectangle bound, unsigned int texture_id);

/**
 * @brief delete button
 * 
 * @param button 
 */
void	haven_gui_button_delete(haven_gui_button *button);

/**
 * @brief button_logic
 * 
 * @param button 
 * @param mouse_pos 
 * @return true 
 * @return false 
 */
bool	haven_gui_button_logic(haven_gui_button *button, Vector2 mouse_pos, bool (*hover_action)(unsigned int), unsigned int arg);

/**
 * @brief button_drawing
 * 
 * @param button 
 * @param texture 
 * @param font 
 */
void	haven_gui_button_draw(haven_gui_button *button, const Texture2D *texture, const Font font);

#endif