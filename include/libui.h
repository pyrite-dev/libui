#ifndef __LIBUI_H__
#define __LIBUI_H__

#include <libui_machdep.h>

#define LIBUIDEF extern

#ifdef __cplusplus
extern "C" {
#endif

#if defined(LIBUI_SOURCE)
typedef struct libui_ {
	libui_machdep_t machdep;
	int x;
	int y;
	int width;
	int height;
} libui_t;
#else
typedef void libui_t;
#endif

LIBUIDEF void libui_init(void);
LIBUIDEF int libui_machdep_create(libui_t* ui, const char* title, int x, int y, int width, int height);
LIBUIDEF void libui_machdep_destroy(libui_t* ui);
LIBUIDEF libui_t* libui_create(const char* title, int x, int y, int width, int height);
LIBUIDEF void libui_loop(libui_t* ui);
LIBUIDEF void libui_destroy(libui_t* ui);
LIBUIDEF void libui_layout(libui_t* ui);

#ifdef __cplusplus
}
#endif

#endif
