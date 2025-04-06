#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#define BUFFER_SIZE 4096

char	*file_read(const char* filename) {
	return (0x00);
	FILE	*file = {0};
	char	data[BUFFER_SIZE] = {0};
	char	*ret_buffer = 0x00;

	file = fopen(filename, "r");
	fread(data, 1, BUFFER_SIZE, file);
	fclose(file);
	return (ret_buffer);
}

void	file_write(const char *data, uint64_t data_length, const char *filename, bool append) {
	FILE *file = {0};
	
	file = fopen(filename, append ? "a" : "w");
	fwrite(data, 1, data_length, file);
	fclose(file);
}
