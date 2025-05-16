#include <libui.h>

int main(){
	libui_t* ui = libui_create("test", 0, 0, 200, 200);
	libui_widget_t* w;
	int i;

	for(i = 0; i < (200 / 50) * (200 / 25); i++){
		w = libui_new_widget(ui);
		w->text = "Test";
		w->type = LIBUI_BUTTON;
		w->origin_x = LIBUI_LEFT;
		w->origin_y = LIBUI_TOP;
		w->x = (i % 4) * 50;
		w->y = (i / 4) * 25;
		w->width = 50;
		w->height = 25;
	}

	libui_loop(ui);
	libui_destroy(ui);
}
