#include <sparse_octree.h>
#include <dynamic_array.h>
#include <stdio.h>

int main(void) {

	dyn_array_t	*dyn_array;
	svo_t		*svo;

	//dynarray
	printf("init_dyn_array ...");
	dyn_array = init_dyn_array(sizeof(int));
	bool check = false;
	for (int i = 0; i < 8; i++) {
		if (((int*)dyn_array->arena)[i]) {
			check = true;
			break;
		}
	}
	if (dyn_array->size || dyn_array->capacity != 8 * dyn_array->data_size || check) {
		printf(" BAD\n");
	} else {
		printf(" OK\n");
	}
	
	printf("dyn_add_elem ...");
	dyn_add_elem(dyn_array, (void *)5);
	dyn_add_elem(dyn_array, (void *)4);
	printf ("%i %i", *((int *)dyn_array->arena), dyn_array->size);
	if (5 == ((int *)dyn_array->arena)[0] && dyn_array->size == 2) {	
		printf(" OK\n");
	} else {
		printf(" BAD\n");
	}

	
	printf("dyn_del_elem ...");
	dyn_del_elem(dyn_array, 0);
	printf ("%i %i", *((int *)dyn_array->arena), dyn_array->size);
	if (4 == ((int *)dyn_array->arena)[0] && dyn_array->size == 1) {	
		printf(" OK\n");
	} else {
		printf(" BAD\n");
	}

	printf("destroy_dyn_array ...");
	destroy_dyn_array(dyn_array);
	printf(" OK\n");

	//svo
	

	return (0);
}
