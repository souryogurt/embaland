#ifndef EMBALAND_PLATFORM_H_
#define EMBALAND_PLATFORM_H_ 1
/**
 * @file
 * The header of embaland platform definitions.
 *
 * This header file defines platform specific types and attributes.
 */

/**
 * @def EMB_STATIC
 * Must be defined by applications that are linking against the static version
 * of the embaland library
 */

#if defined(EMB_STATIC) && defined(_EMB_BUILD_DLL)
#error "You must not have both EMB_STATIC and _EMB_BUILD_DLL defined"
#endif

/**
 * @def EMB_API
 * Placed before the return type in function declarations to declare public API
 * functions for export.
 */

/**
 * @def EMB_LOCAL
 * Placed before the return type in function declarations to declare local
 * functions not indented for export.
 */

#if (defined(_WIN32) || defined(__CYGWIN__)) && defined(_EMB_BUILD_DLL)
/* We are building embaland as a Win32 DLL */
#define EMB_API __declspec(dllexport)
#define EMB_LOCAL
#elif (defined(_WIN32) || defined(__CYGWIN__)) && !defined(EMB_STATIC)
/* We are calling embaland as Win32 DLL */
#define EMB_API __declspec(dllimport)
#define EMB_LOCAL
#elif defined(__GNUC__) && defined(_EMB_BUILD_DLL)
/* We are building embaland as a shared library */
#define EMB_API __attribute__((visibility("default")))
#define EMB_LOCAL __attribute__((visibility("hidden")))
#else
/* We are building or calling embaland as a static library */
#define EMB_API
#define EMB_LOCAL
#endif

/**
 * @def EMB_CALL
 * Placed after the return type in function declarations to specify calling
 * convention.
 */

#if defined(_WIN32)
#define EMB_CALL __stdcall
#else
#define EMB_CALL
#endif

#include <stddef.h>

/**
 * @def EMB_NO_STDINT_H
 * Controls definition of standard integer types.
 *
 * If the EMB_NO_STDINT_H macro is defined by the application at compile time,
 * extended integer types used by the embaland API, such as uint8_t, must also
 * be defined by the application. Otherwise, the embaland headers will not
 * compile. If EMB_NO_STDINT_H is not defined, the system <stdint.h> is used to
 * define these types. There is a fallback path when Microsoft Visual Studio
 * version 2008 and earlier versions are detected at compile time.
 */

#if !defined(EMB_NO_STDINT_H)
#if defined(_MSC_VER) && (_MSC_VER < 1600)
typedef signed __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef signed __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef signed __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif
#endif /* !defined(EMB_NO_STDINT_H) */
#endif
