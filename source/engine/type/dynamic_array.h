
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>

#ifndef ARRAY_ALLOC_STEP
# define ARRAY_ALLOC_STEP 1.25f
#endif

/**
 * @brief dynamic array data structure
 * 
 */
typedef struct dynamic_array {
	char	data_size;// Size of each element (in bytes)
	void*	data;
	size_t	size;// Number of elements in the array
	size_t	capacity;// Total capacity of the array (in bytes)
	size_t	alignement;//Alignment requirement (must be a power of two)
}	dynamic_array;

/**
 * @brief 
 * 
 * @param array 
 * @param element 
 */
static void	dynamic_array_add(dynamic_array *array, const void *element) {
	assert(array);
	if ((array->size + 1) * array->data_size >= array->capacity) {
		const uint32_t new_capacity = (unsigned int)(array->capacity * ARRAY_ALLOC_STEP);
		void *tmp = _aligned_malloc(new_capacity, array->alignement);
		assert(tmp);

		memcpy(tmp, array->data, array->size * array->data_size);

		_aligned_free(array->data);
		array->data = tmp;
		array->capacity = new_capacity;
	}

	memcpy((unsigned char *)array->data + array->size * array->data_size, element, array->data_size);
	array->size++;
}

/**
 * @brief 
 * 
 * @param array 
 * @param idx 
 */
static void	dynamic_array_del(dynamic_array *array, const unsigned int idx) {
	assert(array);
	if (idx >= array->size) return;

	memmove((unsigned char *)array->data + idx * array->data_size,
		(unsigned char *)array->data + (idx + 1) * array->data_size,
		(array->size - idx - 1) * array->data_size);
	array->size--;

	if (array->size * array->data_size < array->capacity * 0.5 && array->capacity > 8 * array->data_size) {
		const uint32_t new_capacity = (unsigned int)(array->capacity * 0.5);
		void *tmp = _aligned_malloc(new_capacity, array->alignement);
		assert(tmp);

		memcpy(tmp, array->data, array->size * array->data_size);

		_aligned_free(array->data);
		array->data = tmp;
		array->capacity = new_capacity;
	}
}

/**
 * @brief 
 * 
 * @param array 
 */
static void	dynamic_array_clear(dynamic_array *array) {
	assert(array);
	memset(array->data, 0, array->capacity);
	array->size = 0;
}

/**
 * @brief 
 * 
 * @param data_size 
 * @param alignement 
 * @return dynamic_array* 
 */
static dynamic_array*	dynamic_array_init(const char data_size, const size_t alignement) {
	dynamic_array* array = (dynamic_array *)malloc(sizeof(dynamic_array));
	assert(array);

	array->data = _aligned_malloc(data_size * 8, array->alignement);
	assert(array->data);

	array->capacity = data_size * 8;
	array->data_size = data_size;
	array->size = 0;

	haven_darray_clear(array);
	return array;
}

/**
 * @brief 
 * 
 * @param array 
 */
static void	dynamic_array_destroy(dynamic_array* array) {
	assert(array);
	_aligned_free(array->data);
	free(array);
}

/**
 * @brief 
 * 
 * @param array 
 * @param start 
 * @param end 
 */
static void	dynamic_array_range_remove(dynamic_array* array, const uint32_t start, const uint32_t end) {
	assert(array);
	if (start >= array->size || end >= array->size || start > end) {
	    return;
	}

	const uint32_t num_elements_to_remove = end - start + 1;
	memmove(
		(unsigned char *)array->data + start * array->data_size,
		(unsigned char *)array->data + (end + 1) * array->data_size,
		(array->size - end - 1) * array->data_size
	);
	array->size -= num_elements_to_remove;

	if (array->size * array->data_size < array->capacity * 0.5 && array->capacity > 8 * array->data_size) {
		const uint32_t new_capacity = (unsigned int)(array->capacity * 0.5);
		void *tmp = _aligned_malloc(new_capacity, array->alignement);
		assert(tmp);

		memcpy(tmp, array->data, array->size * array->data_size);

		_aligned_free(array->data);
		array->data = tmp;
		array->capacity = new_capacity;
	}
}

/**
 * @brief 
 * 
 * @param array 
 * @param comparator 
 */
static void	dynamic_array_sort(const dynamic_array *array, int (*comparator)(const void *, const void *)) {
	assert(array);
	qsort(array->data, array->size, array->data_size, comparator);
}

//return a pointer to data idx;
/**
 * @brief 
 * 
 * @param array 
 * @param idx 
 * @return void* 
 */
static void*	dynamic_array_get(const dynamic_array *array, const uint32_t idx) {
	assert(array);
	void *ret = (unsigned char *)array->data + (idx * array->data_size);
	return (ret);
}


/**
 * @brief 
 * 
 * @param array 
 * @param callback 
 */
static void	dynamic_array_foreach(dynamic_array *array, void (*callback)(void *)) {
	assert(array);
	for (uint32_t i = 0; i < array->size; i++) {
		callback(haven_darray_get(array, i));
	}
}
