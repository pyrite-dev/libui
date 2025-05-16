#include <libui.h>

int main(){
	libui_t* ui = libui_create("test", 0, 0, 200, 200);
	libui_widget_t* w;

	w = libui_new_widget(ui);
	w->type = LIBUI_BUTTON;
	w->x = 50;
	w->y = 50;
	w->width = 100;
	w->height = 100;

	libui_loop(ui);
	libui_destroy(ui);
}
