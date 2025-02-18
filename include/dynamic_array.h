#ifndef DYN_ARRAY_H
# define DYN_ARRAY_H

# include <stdlib.h>
# include <string.h>
# include <assert.h>

typedef struct s_dynamic_array {
	void		*arena;
	unsigned int	size;
	unsigned int	capacity;
	unsigned char	data_size;
}	dyn_array_t;

static void	dyn_add_elem(dyn_array_t *array, void *element) {
	if (array->size * array->data_size >= array->capacity - array->data_size) {
		void *tmp = malloc(sizeof(void *) * array->capacity * 2);
		assert(tmp);
		array->capacity *= 2;
		array->size++;
		memcpy(tmp, array->arena, array->size * array->data_size);
		free(array->arena);
		array->arena = tmp;
	}
	switch (array->data_size) {
		case (sizeof(char)): {
			((char *)array->arena)[array->size] = (char)element;
			break;
		}
		case (sizeof(short)): {
			((short *)array->arena)[array->size] = (short)element;
			break;
		}
		case (sizeof(int)): {
			((int *)array->arena)[array->size] = (int)element;
			break;
		}
		case (sizeof(long long)): {
			((long long *)array->arena)[array->size] = (long long)element;
			break;
		}
		default:break;
	}
	array->size++;
}

static void	dyn_del_elem(dyn_array_t *array, unsigned int idx) {
	memcpy(array->arena + idx * array->data_size, array->arena + (idx + 1) * array->data_size, array->data_size * (array->size - 1));
	array->size--;
	if (array->size * array->data_size < array->capacity * 0.5 && array->capacity > 8 * array->data_size) {
		void *tmp = malloc(sizeof(void *) * array->capacity * 0.5);
		assert(tmp);
		array->capacity *= 0.5;
		memcpy(tmp, array->arena, array->size * array->data_size);
		free(array->arena);
		array->arena = tmp;
	}
}

static void	dyn_clear(dyn_array_t *array) {
	for (int i = 0; i < array->capacity / array->data_size; i++) {
		switch (array->data_size) {
			case (sizeof(char)): {
				((char *)array->arena)[i] = (char)0;
				break;
			}
			case (sizeof(short)): {
				((short *)array->arena)[i] = (short)0;
				break;
			}
			case (sizeof(int)): {
				((int *)array->arena)[i] = (int)0;
				break;
			}
			case (sizeof(long long)): {
				((long long *)array->arena)[i] = (long long)0;
				break;
			}
			default:break;
		}
	}
}

static dyn_array_t	*init_dyn_array(unsigned char data_size) {
	dyn_array_t	*array;
	array = (dyn_array_t*)malloc(sizeof(dyn_array_t));
	assert(array);

	array->arena = malloc(data_size * 8);
	assert(array->arena);

	array->capacity = data_size * 8;
	array->data_size = data_size;
	array->size = 0;

	dyn_clear(array);
	return (array);
}

static void	destroy_dyn_array(dyn_array_t *array) {
	dyn_clear(array);
	free(array->arena);
	free(array);
}

#endif
