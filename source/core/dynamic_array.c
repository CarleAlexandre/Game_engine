# include <stdlib.h>
# include <string.h>
# include <assert.h>

#ifndef ALLOC_STEP
# define ALLOC_STEP 1.25f
#endif

/**
 * @brief dynamic array data structure
 * 
 */
typedef struct haven_darray_s {
	void*		data;
	unsigned int	size;// Number of elements in the array
	unsigned int	capacity;// Total capacity of the array (in bytes)
	unsigned char	data_size;// Size of each element (in bytes)
}	haven_darray_t;

void	haven_darray_add(haven_darray_t *array, void *element) {
	if ((array->size + 1) * array->data_size >= array->capacity) {
		unsigned int new_capacity = (unsigned int)(array->capacity * ALLOC_STEP);
		void *tmp = malloc(new_capacity);
		assert(tmp);

		memcpy(tmp, array->data, array->size * array->data_size);

		free(array->data);
		array->data = tmp;
		array->capacity = new_capacity;
	}

	memcpy((unsigned char *)array->data + array->size * array->data_size, element, array->data_size);
	array->size++;
}

void	haven_darray_del(haven_darray_t *array, unsigned int idx) {
	if (idx >= array->size) return;

	memmove((unsigned char *)array->data + idx * array->data_size,
		(unsigned char *)array->data + (idx + 1) * array->data_size,
		(array->size - idx - 1) * array->data_size);
	array->size--;

	if (array->size * array->data_size < array->capacity * 0.5 && array->capacity > 8 * array->data_size) {
		unsigned int new_capacity = (unsigned int)(array->capacity * 0.5);
		void *tmp = malloc(new_capacity);
		assert(tmp);

		memcpy(tmp, array->data, array->size * array->data_size);

		free(array->data);
		array->data = tmp;
		array->capacity = new_capacity;
	}
}

void	haven_darray_clear(haven_darray_t *array) {
	memset(array->data, 0, array->capacity);
	array->size = 0;
}

haven_darray_t*	init_dyn_array(unsigned char data_size) {
	haven_darray_t* array = (haven_darray_t *)malloc(sizeof(haven_darray_t));
	assert(array);

	array->data = malloc(data_size * 8);
	assert(array->data);

	array->capacity = data_size * 8;
	array->data_size = data_size;
	array->size = 0;

	dyn_clear(array);
	return array;
}

void	haven_darray_destroy(haven_darray_t* array) {
	free(array->data);
	free(array);
}

void	haven_darray_range_remove(haven_darray_t* array, unsigned int start, unsigned int end) {
	if (start >= array->size || end >= array->size || start > end) {
	    return;
	}

	unsigned int num_elements_to_remove = end - start + 1;
	memmove(
		(unsigned char *)array->data + start * array->data_size,
		(unsigned char *)array->data + (end + 1) * array->data_size,
		(array->size - end - 1) * array->data_size
	);
	array->size -= num_elements_to_remove;

	if (array->size * array->data_size < array->capacity * 0.5 && array->capacity > 8 * array->data_size) {
		unsigned int new_capacity = (unsigned int)(array->capacity * 0.5);
		void *tmp = malloc(new_capacity);
		assert(tmp);

		memcpy(tmp, array->data, array->size * array->data_size);

		free(array->data);
		array->data = tmp;
		array->capacity = new_capacity;
	}
}

void	haven_darray_range_move(haven_darray_t* array, unsigned int src_start, unsigned int src_end, unsigned int dst) {
	if (src_start >= array->size || src_end >= array->size || src_start > src_end || dst >= array->size) {
		return;
	}

	unsigned int num_elements = src_end - src_start + 1;

	void *temp = malloc(num_elements * array->data_size);
	assert(temp);

	memcpy(temp, (unsigned char *)array->data + src_start * array->data_size, num_elements * array->data_size);

	if (dst < src_start) {
	memmove(
		(unsigned char *)array->data + (dst + num_elements) * array->data_size,
		(unsigned char *)array->data + dst * array->data_size,
		(src_start - dst) * array->data_size
	);
	} else if (dst > src_end) {
		memmove(
			(unsigned char *)array->data + src_start * array->data_size,
			(unsigned char *)array->data + (src_end + 1) * array->data_size,
			(dst - src_end - 1) * array->data_size
		);
	}

	memcpy((unsigned char *)array->data + dst * array->data_size, temp, num_elements * array->data_size);

	free(temp);
}

void	haven_darray_sort(haven_darray_t *array, int (*comparator)(const void *, const void *)) {
	qsort(array->data, array->size, array->data_size, comparator);
}

//return a pointer to data idx;
void*	haven_darray_get(haven_darray_t *array,unsigned int idx) {
	return ((array->data + idx * array->data_size));
}
