/**
 * @file
 * Mock of string module
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "string.h"

#include <cgreen/mocks.h>

EMB_LOCAL const char *emb_string_fmtv(const char *fmt, va_list args)
{
	return (const char *)mock(fmt, args);
}
