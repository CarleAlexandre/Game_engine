#include <engine.hpp>

int	check_file(char *magic, const char *filename) {
	if (!FileExists(filename))
		return (-1);
	int filesize = 0;
	unsigned char *data = LoadFileData(filename, &filesize);

	
	return (0);
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
