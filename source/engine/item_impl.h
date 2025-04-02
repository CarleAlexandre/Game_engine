#ifndef ITEM_IMPL
# define ITEM_IMPL

typedef struct s_item_stats {
	int	material;
	int	damage;
	int	durability;
	int	effect;
	int	rarity;
}	item_stats_t;

typedef struct item_data {
	int	type;
	int	texture_id;
	int	max_stack;
	int	size;
}	item_data_t;

typedef struct s_item {
	item_data_t	data;
	item_stats_t	stats;
}	item_t;

typedef	struct s_inventory {
	int		money;
	item_t		item[64];
	unsigned int	size;
}	inventory_t;

typedef struct s_tool_bar{
	int	current_item;
	item_t	item[9];
}	tool_bar_t;

#endif