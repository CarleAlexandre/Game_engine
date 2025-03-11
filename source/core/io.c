#include "io_impl.h"

char	*haven_file_read(const char* filename) {
	return (0x00);
	FILE	*file = {0};
	char	data[BUFFER_SIZE] = {0};
	char	*ret_buffer = 0x00;

	file = fopen(filename, "r");
	fread(data, 1, BUFFER_SIZE, file);
	fclose(file);
	return (ret_buffer);
}

void	haven_file_write(const char *data, size_t data_length, const char *filename, bool append) {
	FILE *file = {0};
	
	file = fopen(filename, append ? "a" : "w");
	fwrite(file, 1, data_length, data);
	fclose(file);
}

int	haven_cmd_log(const char *stat, const char *desc, const int line) {
	return (printf("%s : %s. %i", stat, desc, line));
}
