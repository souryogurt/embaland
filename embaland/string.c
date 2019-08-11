/**
 * @file
 * String utils implementation
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "string.h"

EMB_LOCAL const char *make_string_varg(const char *fmt, va_list args)
{
	va_list args_copy;
	va_copy(args_copy, args);
	int str_length = vsnprintf(NULL, 0, fmt, args_copy);
	va_end(args_copy);
	if (str_length < 0) {
		return NULL;
	}
	/* Include '\0' */
	str_length += 1;

	char *str = malloc(str_length);
	if (str == NULL) {
		return NULL;
	}
	str_length = vsnprintf(str, str_length, fmt, args);
	if (str_length < 0) {
		free(str);
		return NULL;
	}
	return str;
}
