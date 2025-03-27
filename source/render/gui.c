#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <raylib.h>
#define RAYLIB_IMPLEMENTATION
#include <raygui.h>
#include <raymath.h>
#include <string.h>

//button texture should be at least of 64x64 with 32x64 for each frame

#define BUTTON_TEX_WIDTH 64
#define BUTTON_TEX_HEIGHT 31

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

haven_gui_button	*haven_gui_button_create(const char *text, Rectangle bound, unsigned int texture_id) {
	haven_gui_button *button = 0x00;
	button = malloc(sizeof(haven_gui_button));
	assert(button);
	button->str = strdup(text);
	assert(button->str);

	button->texture_id = texture_id;
	button->bound = bound;
	button->state = 0;
	button->collision = false;
	return (button);
}

void	haven_gui_button_delete(haven_gui_button *button) {
	free(button->str);
	free(button);
}

bool	haven_gui_button_logic(haven_gui_button *button, Vector2 mouse_pos, bool (*hover_action)(unsigned int), unsigned int arg) {
	if (BUTTON_DOWN != button->state) {
		if (CheckCollisionPointRec(mouse_pos, button->bound)) {
			button->collision = true;
			if (BUTTON_HOVER != button->state) {
				button->state = BUTTON_HOVER;
				hover_action(arg);
			}
		} else {
			button->collision = false;
			button->state = BUTTON_DEFAULT;
		}
	}
	if (true == button->collision && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		button->state = BUTTON_DOWN;
	}
	if (BUTTON_DOWN == button->state && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		button->state = BUTTON_RELEASE;
	}
	if (BUTTON_RELEASE == button->state && true == button->collision) {
		button->state = BUTTON_DEFAULT;
		return (true);
	}
	return (false);
}

void	haven_gui_button_draw(haven_gui_button *button, const Texture2D *texture, const Font font) {
	DrawTexturePro(texture[button->texture_id],
		(Rectangle){0, BUTTON_TEX_HEIGHT * button->state, BUTTON_TEX_WIDTH, BUTTON_TEX_HEIGHT},
		button->bound, (Vector2){0, 0}, 0, WHITE);
	DrawTextEx(font, button->str,
		Vector2AddValue((Vector2){button->bound.x, button->bound.y}, 5),
		button->bound.height - 10, 0, WHITE);
}