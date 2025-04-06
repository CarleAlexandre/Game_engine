#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include <string.h>

int	cmd_log(const char *stat, const char *desc, const int line) {
	return (printf("%s : %s. %i", stat, desc, line));
}