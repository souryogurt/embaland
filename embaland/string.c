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

static char *emb_string_allocv(int *length, const char *fmt, va_list args)
{
	va_list args_copy;
	va_copy(args_copy, args);
	*length = vsnprintf(NULL, 0, fmt, args_copy);
	va_end(args_copy);
	if (*length < 0) {
		return NULL;
	}
	/* Include '\0' */
	*length += 1;

	return malloc(*length);
}

EMB_LOCAL const char *emb_string_fmtv(const char *fmt, va_list args)
{
	int length = 0;
	char *str = emb_string_allocv(&length, fmt, args);
	if (str == NULL) {
		return NULL;
	}
	if (vsnprintf(str, length, fmt, args) < 0) {
		free(str);
		return NULL;
	}
	return str;
}

EMB_LOCAL const char *emb_string_fmt(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	const char *result = emb_string_fmtv(fmt, args);
	va_end(args);
	return result;
}
