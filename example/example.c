#include <libui.h>

int main(){
	libui_t* ui = libui_create("test", 0, 0, 200, 200);
	libui_widget_t* w;

	w = libui_new_widget(ui);
	w->text = "Test button";
	w->type = LIBUI_BUTTON;
	w->origin_x = LIBUI_RIGHT;
	w->origin_y = LIBUI_BOTTOM;
	w->x = 0;
	w->y = 0;
	w->width = 100;
	w->height = 100;

	libui_loop(ui);
	libui_destroy(ui);
}
