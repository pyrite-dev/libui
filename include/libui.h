#ifndef __LIBUI_H__
#define __LIBUI_H__

#include <libui_machdep.h>

#define LIBUIDEF extern

#ifdef __cplusplus
extern "C" {
#endif

typedef struct libui_ {
	libui_machdep_t machdep;
} libui_t;

LIBUIDEF void libui_init(void);
LIBUIDEF libui_t* libui_create(const char* title, int x, int y);
LIBUIDEF void libui_destroy(libui_t* ui);

#ifdef __cplusplus
}
#endif

#endif
