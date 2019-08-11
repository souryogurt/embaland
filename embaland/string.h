#ifndef EMBALAND_STRING_H
#define EMBALAND_STRING_H 1

#include "platform.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns new formated string.
 * @param fmt
 * @returns new allocatied string, or NULL otherwise
 */
EMB_LOCAL const char *emb_string_fmt(const char *fmt, ...);

/**
 * Allocates new string with the content specifiet by @a fmt and @a args.
 * @param fmt is format of string content
 * @param args is variable list of arguments
 * @returns new allocated string, or NULL otherwise
 */
EMB_LOCAL const char *emb_string_fmtv(const char *fmt, va_list args);

#ifdef __cplusplus
}
#endif
#endif
