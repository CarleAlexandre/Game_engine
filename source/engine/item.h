#ifndef HAVEN_ITEM_H
# define HAVEN_ITEM_H

#define INVENTORY_SIZE 64
#define TOOL_BAR_SIZE 8

typedef struct item_stats {
	int	material;
	int	damage;
	int	durability;
	int	effect;
	int	rarity;
}	item_stats;

typedef struct item_data {
	int	type;
	int	texture_id;
	int	max_stack;
	int	size;
}	item_data;

typedef struct item {
	item_data	data;
	item_stats	stats;
}	item;

typedef struct tool_bar{
	int	current_item;
	item	item[TOOL_BAR_SIZE];
}	tool_bar;

typedef	struct inventory {
	int		money;
	tool_bar	tool;
	item		item[INVENTORY_SIZE];
}	inventory;

#endif