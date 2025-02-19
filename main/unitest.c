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
	
	printf("dyn_add_elem ...\n");
	printf("%i\n", dyn_array->capacity / dyn_array->data_size);
	dyn_add_elem(dyn_array, (void *)5);
	dyn_add_elem(dyn_array, (void *)4);
	dyn_add_elem(dyn_array, (void *)4);
	dyn_add_elem(dyn_array, (void *)4);
	dyn_add_elem(dyn_array, (void *)4);
	dyn_add_elem(dyn_array, (void *)4);
	printf("%i\n", dyn_array->capacity / dyn_array->data_size);
	dyn_add_elem(dyn_array, (void *)4);
	printf("%i\n", dyn_array->capacity / dyn_array->data_size);
	dyn_add_elem(dyn_array, (void *)4);
	dyn_add_elem(dyn_array, (void *)4);
	dyn_add_elem(dyn_array, (void *)4);
	dyn_add_elem(dyn_array, (void *)4);
	dyn_add_elem(dyn_array, (void *)4);
	dyn_add_elem(dyn_array, (void *)4);
	dyn_add_elem(dyn_array, (void *)4);
	dyn_add_elem(dyn_array, (void *)4);
	dyn_add_elem(dyn_array, (void *)4);
	dyn_add_elem(dyn_array, (void *)4);
	printf("%i\n", dyn_array->capacity / dyn_array->data_size);

	printf ("%i %i\n", *((int *)dyn_array->arena), dyn_array->size);
	if (5 == ((int *)dyn_array->arena)[0] && dyn_array->size == 22) {
		printf(" OK\n");
	} else {
		printf(" BAD\n");
	}

	
	printf("dyn_del_elem ...");
	dyn_del_elem(dyn_array, 0);
	printf ("%i %i", *((int *)dyn_array->arena), dyn_array->size);
	if (4 == ((int *)dyn_array->arena)[0] && dyn_array->size == 21) {
		printf(" OK\n");
	} else {
		printf(" BAD\n");
	}

	printf("destroy_dyn_array ...");
	destroy_dyn_array(dyn_array);
	printf(" OK\n");

	//svo
	printf("init_svo ...");
	svo = init_svo(4, 2);
	if (svo) {
		printf(" OK\n");
	} else {
		printf(" BAD\n");
	}

	printf("svo_insert ...");
	svo_insert((Vector3){0, 0, 0}, (void *)4, svo);
	svo_insert((Vector3){0, 0, 1}, (void *)5, svo);
	svo_insert((Vector3){0, 1, 0}, (void *)6, svo);
	svo_insert((Vector3){1, 0, 0}, (void *)7, svo);
	svo_insert((Vector3){0, 1, 1}, (void *)8, svo);
	svo_insert((Vector3){1, 0, 1}, (void *)9, svo);
	svo_insert((Vector3){1, 1, 1}, (void *)10, svo);
	if (svo->element == 7) {
		printf(" OK\n");
	} else {
		printf(" BAD\n");
	}

	printf("svo_get_node ...\n");
	int res = (int)svo_get_node((Vector3){0, 0, 0}, svo)->data;
	printf ("%i %i\n", (int)svo_get_node((Vector3){0, 0, 0}, svo)->data, svo->element);
	printf ("%i %i\n", (int)svo_get_node((Vector3){0, 0, 1}, svo)->data, svo->element);
	printf ("%i %i\n", (int)svo_get_node((Vector3){0, 1, 0}, svo)->data, svo->element);
	printf ("%i %i\n", (int)svo_get_node((Vector3){1, 0, 0}, svo)->data, svo->element);
	printf ("%i %i\n", (int)svo_get_node((Vector3){0, 1, 1}, svo)->data, svo->element);
	printf ("%i %i\n", (int)svo_get_node((Vector3){1, 0, 1}, svo)->data, svo->element);
	printf ("%i %i\n", (int)svo_get_node((Vector3){1, 1, 1}, svo)->data, svo->element);
	if (res == 4) {
		printf("OK\n");
	} else {
		printf("BAD\n");
	}

	return (0);
}
