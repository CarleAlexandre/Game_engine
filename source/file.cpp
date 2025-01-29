#include <engine.hpp>

int check_file(char *magic, const char *filename) {
	int ret = 0;
	if (!FileExists(filename))
		ret = -1;
	return (ret);
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
