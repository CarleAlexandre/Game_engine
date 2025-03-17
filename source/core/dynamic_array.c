
# include <stdint.h>
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
	char	data_size;// Size of each element (in bytes)
	void*	data;
	size_t	size;// Number of elements in the array
	size_t	capacity;// Total capacity of the array (in bytes)
	size_t	alignement;//Alignment requirement (must be a power of two)
}	haven_darray_t;

void	haven_darray_add(haven_darray_t *array, const void *element) {
	if ((array->size + 1) * array->data_size >= array->capacity) {
		const uint32_t new_capacity = (unsigned int)(array->capacity * ALLOC_STEP);
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

void	haven_darray_del(haven_darray_t *array, const unsigned int idx) {
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

void	haven_darray_clear(haven_darray_t *array) {
	memset(array->data, 0, array->capacity);
	array->size = 0;
}

haven_darray_t*	init_dyn_array(const char data_size, const size_t alignement) {
	haven_darray_t* array = (haven_darray_t *)malloc(sizeof(haven_darray_t));
	assert(array);

	array->data = _aligned_malloc(data_size * 8, array->alignement);
	assert(array->data);

	array->capacity = data_size * 8;
	array->data_size = data_size;
	array->size = 0;

	haven_darray_clear(array);
	return array;
}

void	haven_darray_destroy(haven_darray_t* array) {
	_aligned_free(array->data);
	free(array);
}

void	haven_darray_range_remove(haven_darray_t* array, const uint32_t start, const uint32_t end) {
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

/*
void	haven_darray_range_move(const haven_darray_t* array, const uint32_t src_start, const uint32_t src_end, const uint32_t dst) {
	if (src_start >= array->size || src_end >= array->size || src_start > src_end || dst >= array->size) {
		return;
	}

	const uint32_t num_elements = src_end - src_start + 1;

	void *temp = _aligned_malloc(num_elements * array->data_size, array->alignement);
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

	_aligned_free(temp);
}
*/

void	haven_darray_sort(const haven_darray_t *array, int (*comparator)(const void *, const void *)) {
	qsort(array->data, array->size, array->data_size, comparator);
}

//return a pointer to data idx;
void*	haven_darray_get(const haven_darray_t *array, const uint32_t idx) {
	void *ret = (unsigned char *)array->data + (idx * array->data_size);
	return (ret);
}


void	haven_darray_foreach(haven_darray_t *array, void (*callback)(void *)) {
	for (uint32_t i = 0; i < array->size; i++) {
		callback(haven_darray_get(array, i));
	}
}

#ifdef HAVEN_UNIT_TEST

#include <assert.h>

static void test_darray_init() {
	haven_darray_t array;
	haven_darray_init(&array, sizeof(int), 4, 8);
	
	assert(array.data != NULL);
	assert(array.size == 0);
	assert(array.capacity == 8);
	assert(array.data_size == sizeof(int));
	assert(array.alignement == 4);
	
	haven_darray_free(&array);
}

static void test_darray_push() {
	haven_darray_t array;
	haven_darray_init(&array, sizeof(int), 4, 2);
	
	int values[] = {1, 2, 3, 4};
	
	// Test pushing within initial capacity
	haven_darray_push(&array, &values[0]);
	haven_darray_push(&array, &values[1]);
	assert(array.size == 2);
	assert(array.capacity == 2);
	
	// Test capacity doubling
	haven_darray_push(&array, &values[2]);
	assert(array.size == 3);
	assert(array.capacity == 4);
	
	// Verify values
	int *data = array.data;
	assert(data[0] == 1);
	assert(data[1] == 2);
	assert(data[2] == 3);
	
	haven_darray_free(&array);
}

static void test_darray_pop() {
	haven_darray_t array;
	haven_darray_init(&array, sizeof(int), 4, 4);
	
	int values[] = {1, 2, 3};
	for (int i = 0; i < 3; i++) {
		haven_darray_push(&array, &values[i]);
	}
	
	int popped;
	haven_darray_pop(&array, &popped);
	assert(popped == 3);
	assert(array.size == 2);
	
	haven_darray_free(&array);
}

static void test_darray_sort() {
	haven_darray_t array;
	haven_darray_init(&array, sizeof(int), 4, 4);
	
	int values[] = {3, 1, 4, 2};
	for (int i = 0; i < 4; i++) {
		haven_darray_push(&array, &values[i]);
	}
	
	int compare(const void *a, const void *b) {
		return (*(int*)a - *(int*)b);
	}
	
	haven_darray_sort(&array, compare);
	
	int *data = array.data;
	assert(data[0] == 1);
	assert(data[1] == 2); 
	assert(data[2] == 3);
	assert(data[3] == 4);
	
	haven_darray_free(&array);
}

void haven_darray_run_tests() {
	test_darray_init();
	test_darray_push();
	test_darray_pop();
	test_darray_sort();
	printf("All dynamic array tests passed!\n");
}

#endif // HAVEN_UNIT_TEST

	
