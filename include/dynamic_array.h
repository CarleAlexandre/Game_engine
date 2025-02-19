#ifndef DYN_ARRAY_H
# define DYN_ARRAY_H

# include <stdlib.h>
# include <string.h>
# include <assert.h>

#ifndef ALLOC_STEP
# define ALLOC_STEP 1.25f
#endif

typedef struct s_dynamic_array {
    void *arena;               // Pointer to the data
    unsigned int size;         // Number of elements in the array
    unsigned int capacity;     // Total capacity of the array (in bytes)
    unsigned char data_size;   // Size of each element (in bytes)
} dyn_array_t;

static void	dyn_add_elem(dyn_array_t *array, void *element) {
	if ((array->size + 1) * array->data_size >= array->capacity) {
		unsigned int new_capacity = (unsigned int)(array->capacity * ALLOC_STEP);
		void *tmp = malloc(new_capacity);
		assert(tmp);

		memcpy(tmp, array->arena, array->size * array->data_size);

		free(array->arena);
		array->arena = tmp;
		array->capacity = new_capacity;
	}

	memcpy((unsigned char *)array->arena + array->size * array->data_size, element, array->data_size);
	array->size++;
}

static void	dyn_del_elem(dyn_array_t *array, unsigned int idx) {
	if (idx >= array->size) {
	return;
	}

	memmove((unsigned char *)array->arena + idx * array->data_size,
		(unsigned char *)array->arena + (idx + 1) * array->data_size,
		(array->size - idx - 1) * array->data_size);
	array->size--;

	if (array->size * array->data_size < array->capacity * 0.5 && array->capacity > 8 * array->data_size) {
		unsigned int new_capacity = (unsigned int)(array->capacity * 0.5);
		void *tmp = malloc(new_capacity);
		assert(tmp);

		memcpy(tmp, array->arena, array->size * array->data_size);

		free(array->arena);
		array->arena = tmp;
		array->capacity = new_capacity;
	}
}

static void	dyn_clear(dyn_array_t *array) {
	memset(array->arena, 0, array->capacity);
	array->size = 0;
}

static dyn_array_t	*init_dyn_array(unsigned char data_size) {
	dyn_array_t *array = (dyn_array_t *)malloc(sizeof(dyn_array_t));
	assert(array);

	array->arena = malloc(data_size * 8);
	assert(array->arena);

	array->capacity = data_size * 8;
	array->data_size = data_size;
	array->size = 0;

	dyn_clear(array);
	return array;
}

static void	destroy_dyn_array(dyn_array_t *array) {
	free(array->arena);
	free(array);
}

static void dyn_remove_range(dyn_array_t *array, unsigned int start, unsigned int end) {
	if (start >= array->size || end >= array->size || start > end) {
	    return;
	}

	unsigned int num_elements_to_remove = end - start + 1;
	memmove(
		(unsigned char *)array->arena + start * array->data_size,
		(unsigned char *)array->arena + (end + 1) * array->data_size,
		(array->size - end - 1) * array->data_size
	);
	array->size -= num_elements_to_remove;

	if (array->size * array->data_size < array->capacity * 0.5 && array->capacity > 8 * array->data_size) {
		unsigned int new_capacity = (unsigned int)(array->capacity * 0.5);
		void *tmp = malloc(new_capacity);
		assert(tmp);

		memcpy(tmp, array->arena, array->size * array->data_size);

		free(array->arena);
		array->arena = tmp;
		array->capacity = new_capacity;
	}
}

static void dyn_move_range(dyn_array_t *array, unsigned int src_start, unsigned int src_end, unsigned int dst) {
	if (src_start >= array->size || src_end >= array->size || src_start > src_end || dst >= array->size) {
		return;
	}

	unsigned int num_elements = src_end - src_start + 1;

	void *temp = malloc(num_elements * array->data_size);
	assert(temp);

	memcpy(temp, (unsigned char *)array->arena + src_start * array->data_size, num_elements * array->data_size);

	if (dst < src_start) {
	memmove(
		(unsigned char *)array->arena + (dst + num_elements) * array->data_size,
		(unsigned char *)array->arena + dst * array->data_size,
		(src_start - dst) * array->data_size
	);
	} else if (dst > src_end) {
		memmove(
			(unsigned char *)array->arena + src_start * array->data_size,
			(unsigned char *)array->arena + (src_end + 1) * array->data_size,
			(dst - src_end - 1) * array->data_size
		);
	}

	memcpy((unsigned char *)array->arena + dst * array->data_size, temp, num_elements * array->data_size);

	free(temp);
}

static void dyn_sort(dyn_array_t *array, int (*comparator)(const void *, const void *)) {
	qsort(array->arena, array->size, array->data_size, comparator);
}

#endif
