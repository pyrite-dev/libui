#include <libui.h>

int main(){
	libui_t* ui = libui_create("test", 0, 0, 400, 300);
	libui_widget_t* w;

	w = libui_new_widget(ui);
	w->text = "Test";
	w->type = LIBUI_BUTTON;
	w->origin_x = LIBUI_LEFT;
	w->origin_y = LIBUI_BOTTOM;
	w->x = 0;
	w->y = 0;
	w->width = 75;
	w->height = 25;

	libui_loop(ui);
	libui_destroy(ui);
}
