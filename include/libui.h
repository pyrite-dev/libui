#ifndef __LIBUI_H__
#define __LIBUI_H__

#include <libui_machdep.h>

#define LIBUIDEF extern

#ifdef __cplusplus
extern "C" {
#endif

enum LIBUI_WIDGET_TYPE {
	LIBUI_BUTTON = 0
};

enum LIBUI_ORIGIN {
	LIBUI_LEFT = 0,
	LIBUI_RIGHT,
	LIBUI_TOP,
	LIBUI_BOTTOM
};

typedef struct libui_widget_ {
	void* context;
	int id;

	int parent;

	int check_xywh;

	int old_x;
	int old_y;
	int old_width;
	int old_height;

	int origin_x;
	int origin_y;

	int x;
	int y;
	int width;
	int height;

	int ui_x;
	int ui_y;
	int ui_width;
	int ui_height;

	int type;

	char* text;
} libui_widget_t;

#if defined(LIBUI_SOURCE)
typedef struct libui_ {
	libui_machdep_t machdep;
	int x;
	int y;
	int width;
	int height;
	libui_widget_t** widgets;
} libui_t;
#else
typedef void libui_t;
#endif

LIBUIDEF int libui_machdep_create(libui_t* ui, const char* title, int x, int y, int width, int height);
LIBUIDEF void libui_machdep_destroy(libui_t* ui);
LIBUIDEF void libui_machdep_process(libui_t* ui, libui_widget_t* w);

LIBUIDEF void libui_init(void);
LIBUIDEF libui_t* libui_create(const char* title, int x, int y, int width, int height);
LIBUIDEF void libui_process(libui_t* ui);
LIBUIDEF void libui_loop(libui_t* ui);
LIBUIDEF void libui_destroy(libui_t* ui);
LIBUIDEF void libui_layout(libui_t* ui);
LIBUIDEF void libui_geometry(libui_t* ui, int id, int* x, int* y, int* width, int* height);
LIBUIDEF int libui_get_index(libui_t* ui, int id);
LIBUIDEF libui_widget_t* libui_new_widget(libui_t* ui);

#ifdef __cplusplus
}
#endif

#endif
