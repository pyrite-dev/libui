#include "libui.h"

#include <stdlib.h>
#include <string.h>

void libui_layout(libui_t* ui){
}

libui_t* libui_create(const char* title, int x, int y, int width, int height){
	libui_t* ui = malloc(sizeof(*ui));
	memset(ui, 0, sizeof(*ui));

	if(libui_machdep_create(ui, title, x, y, width, height) < 0){
		free(ui);
		return NULL;
	}

	return ui;
}

void libui_destroy(libui_t* ui){
	libui_machdep_destroy(ui);
	free(ui);
}
