#include "libui.h"

#include <stdlib.h>
#include <string.h>

#include <stb_ds.h>

void libui_geometry(libui_t* ui, int id, int* x, int* y, int* width, int* height){
	int ind = libui_get_index(ui, id);
	int p;
	int first = 1;
	int mulx = 1;
	int muly = 1;

	*x = 0;
	*y = 0;
	*width = 0;
	*height = 0;

	if(ind == -1){
		*width = ui->width;
		*height = ui->height;
		return;
	}

	mulx = ui->widgets[ind]->origin_x == LIBUI_LEFT ? 1 : -1;
	muly = ui->widgets[ind]->origin_y == LIBUI_TOP ? 1 : -1;

	p = ui->widgets[ind]->parent;
	do{
		int x2;
		int y2;
		int w2;
		int h2;
		int ind2;
		libui_geometry(ui, p, &x2, &y2, &w2, &h2);
		if(first){
			first = 0;
			if(mulx == -1){
				*x += w2;
				*x -= ui->widgets[ind]->width;
			}
			if(muly == -1){
				*y += h2;
				*y -= ui->widgets[ind]->height;
			}
		}
		*x += x2;
		*y += y2;
		
		ind2 = libui_get_index(ui, p);
		if(ind2 != -1) p = ui->widgets[ind2]->parent;
	}while(p != 0);

	*x += mulx * ui->widgets[ind]->x;
	*y += muly * ui->widgets[ind]->y;
	*width = ui->widgets[ind]->width;
	*height = ui->widgets[ind]->height;
}

void libui_layout_widget(libui_t* ui, int id){
	int ind = libui_get_index(ui, id);
	int x;
	int y;
	int w;
	int h;
	libui_widget_t* uw;
	if(ind == -1) return;

	uw = ui->widgets[ind];
	libui_layout_widget(ui, uw->parent);

	libui_geometry(ui, uw->id, &x, &y, &w, &h);
	uw->ui_x = x;
	uw->ui_y = y;
	uw->ui_width = w;
	uw->ui_height = h;
}

void libui_layout(libui_t* ui){
	int i;
	for(i = 0; i < arrlen(ui->widgets); i++){
		libui_layout_widget(ui, ui->widgets[i]->id);
	}
}

libui_t* libui_create(const char* title, int x, int y, int width, int height){
	libui_t* ui = malloc(sizeof(*ui));
	memset(ui, 0, sizeof(*ui));

	ui->widgets = NULL;
	ui->draw = NULL;

	if(libui_machdep_create(ui, title, x, y, width, height) < 0){
		free(ui);
		return NULL;
	}

	return ui;
}

void libui_set_draw(libui_t* ui, void(*draw)(libui_t*, libui_widget_t*)){
	ui->draw = draw;
}

void libui_destroy(libui_t* ui){
	libui_machdep_destroy(ui);
	free(ui);
}

int libui_get_index(libui_t* ui, int id){
	int i;
	for(i = 0; i < arrlen(ui->widgets); i++){
		if(ui->widgets[i]->id == id) return i;
	}
	return -1;
}

libui_widget_t* libui_new_widget(libui_t* ui){
	libui_widget_t* w = malloc(sizeof(*w));
	int i = 1;
	memset(w, 0, sizeof(*w));

	while(libui_get_index(ui, i) != -1) i++;

	w->parent = 0;

	w->old_x = -1;
	w->old_y = -1;
	w->old_width = -1;
	w->old_height = -1;

	w->origin_x = LIBUI_LEFT;
	w->origin_y = LIBUI_TOP;
	
	w->x = 0;
	w->y = 0;
	w->width = 0;
	w->height = 0;

	w->id = i;
	arrput(ui->widgets, w);

	return w;
}

void libui_process(libui_t* ui){
	int i;

	for(i = 0; i < arrlen(ui->widgets); i++){
		libui_widget_t* w = ui->widgets[i];

		w->check_xywh = 0;

		if(w->old_x != w->ui_x || w->old_y != w->ui_y || w->old_width != w->ui_width || w->old_height != w->ui_height){
			w->check_xywh = 1;
		}

		libui_machdep_process(ui, w);

		w->old_x = w->ui_x;
		w->old_y = w->ui_y;
		w->old_width = w->ui_width;
		w->old_height = w->ui_height;
	}
}
