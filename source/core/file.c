#include <engine.h>

/*
	item_header:
	magic|number of entry|filesize|entry_size

	object_header:
	magic|number of entry|filesize|entry_size

	world_header:
	magic|number of entry|filesize|entry_size

	player_header:
	magic|number of entry|filesize|entry_size
*/

t_token	*get_file_token(char *data, const char *delim) {
	int i = 0;
	t_token token[1000] = {0};
	char *save;
	int size = 0;
	char *str = 0x00;
	char *tok = 0x00;

	str = strtok_r(data, delim, &save);
	assert(str);
	size = save - data;
	tok = malloc(size + 1);
	assert(tok);
	tok[size] = 0x00;
	memcpy(tok, str, size);
	token[0].data = tok;

	while (i < 1000) {
		str = strtok_r(0x00, delim, &save);
		if (!str) break;
		size = save - data;
		tok = malloc(size + 1);
		assert(tok);
		tok[size] = 0x00;
		memcpy(tok, str, size);
		token[0].data = tok;
		i++;
	}
	t_token *ret = (t_token *)malloc(sizeof(t_token) * (i + 1));
	for (int k = 0; k < i; k++) {
		ret[k] = token[k];
	}
	ret[i].data = 0x00;
	return (ret);
}

file_t	*check_file(char *magic, const char *filename) {
	file_t *file = (file_t *)malloc(sizeof(file_t));

	if (!FileExists(filename))
		return (0x00);

	file->size = 0;
	file->data = LoadFileData(filename, &file->size);

	

	if (TextIsEqual("item_UwU", (char *)file->data) == true) {
		file->type = item_filetype;
	}
	if (TextIsEqual("objt_UwU", (char *)file->data) == true) {
		file->type = object_filetype;
	}
	if (TextIsEqual("worldUwU", (char *)file->data) == true) {
		file->type = world_filetype;
	}
	if (TextIsEqual("play_UwU", (char *)file->data) == true) {
		file->type = player_filetype;
	}
	return (file);
}

// typedef struct s_item {
// 	bool is_placable;
// 	int type;
// 	int texture_id;
// 	int material;
// 	int damage;
// 	int durability;
// 	int effect;
// 	int max_stack;
// 	int rarity;
// 	int size;
// }	item_t;

item_t	item_format(const char *filename) {
	
}

char	*format_item(item_t item) {
	
}

void	save_item() {

}


void	create_data_file() {

}
