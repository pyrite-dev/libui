#include <libui.h>

int main(){
	libui_t* ui = libui_create("test", 0, 0, 800, 600);
	libui_loop(ui);
	libui_destroy(ui);
}
