#include "io_impl.h"

char	*file_read(const char* filename) {
	FILE *file;
	char data[BUFFER_SIZE];
	char *ret_buffer = 0x00;

	file = fopen(filename, "r");
	fread(data, 1, BUFFER_SIZE, file);
	fclose(file);
	return (ret_buffer);
}

void	file_write() {
}

void	file_get_header() {

}

char	*file_load(const char *filename) {

}

void	file_unload(char *data) {

}

int	file_save(const char *data, size_t data_length, const char *filename, bool append) {
	FILE *file = {0};
	
	file = fopen(filename, append ? "a" : "w");
	if (!fwrite(file, 1, data_length, data))
		return (-1);
	fclose(file);
	return (0);
}

/*
for terminal use
static FILE	*term_in;
static FILE	*term_out;

void	term_read() {
	term_out = fdopen(1, "w");
}

void	term_write() {
	term_in = fdopen(0, "r");
}
*/
