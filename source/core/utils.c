#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define BUFFER_SIZE 4096

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
	fwrite(data, 1, data_length, file);
	fclose(file);
}

int	haven_cmd_log(const char *stat, const char *desc, const int line) {
	return (printf("%s : %s. %i", stat, desc, line));
}

/*
 *	LEXER 
 */

typedef struct {
	uint32_t	position;
	const char	*input;
}	lexer;

void lexer_init(lexer *l, const char *input) {
	assert(l);
	l->input = input;
	l->position = 0;
}

void lexer_advance(lexer *l) {
	assert(l);
	l->position++;
}

char lexer_peek(lexer *l) {
	assert(l);
	return (l->input[l->position]);
}

char lexer_peek_next(lexer *l) {
	assert(l);
	return (l->input[l->position + 1]);
}

int lexer_match(lexer *l, char expected) {
	assert(l);
	return (l->input[l->position] - expected);
}

void lexer_skip_whitespace(lexer *l) {
	assert(l);
	while (isspace(l->input[l->position])) {
		l->position++;
	}
}

int	lexer_read_int(lexer *l) {
	assert(l);
	int x = 0;
	while (isdigit(l->input[l->position])) {
		x *= 10;
		x += l->input[l->position] - '0';
	}
	return (x);
}

char*	lexer_read_string(lexer *l) {
	assert(l);
	const int max_size = 100;
	char str[max_size];

	int k = 0;
	for (;isalnum(l->input[l->position + k] && k < max_size); k++) {
		str[k] = l->input[l->position + k];
	}	
	str[k] = 0x00;
	
	return(strdup(str));
}

typedef struct {
	uint32_t	id;
	char*		data;
}	token;

typedef struct {
	char**		data;
	uint16_t	size;
}	dictionnary;

void	token_getid(const char *data, const dictionnary *context) {
	assert(context);
	assert(data);
	for(uint32_t i = 0; i < context->size; i++) {
		if (strcmp(data, context->data[i]) == 0x00) {
			break;
		}
	}
}
