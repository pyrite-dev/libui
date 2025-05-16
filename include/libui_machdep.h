#ifndef __LIBUI_MACHDEP_H__
#define __LIBUI_MACHDEP_H__

#include <libui_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(LIBUI_SOURCE)
#if defined(LIBUI_MOTIF)
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>

typedef struct libui_machdep_ {
	XtAppContext context;
	Widget top;
	Widget main;
} libui_machdep_t;
#elif defined(LIBUI_WIN32)
typedef struct libui_machdep_ {
} libui_machdep_t;
#endif
#else
typedef void libui_machdep_t;
#endif

#ifdef __cplusplus
}
#endif

#endif
