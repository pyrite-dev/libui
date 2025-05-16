#include <libui.h>

double deg = 0;
void draw(libui_t* ui, libui_widget_t* w){
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5, 5, -5, 5, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glRotatef(deg,0,0,1);
	glBegin(GL_TRIANGLES);
	glColor3f(1,0,0);
	glVertex2f(0,3);
	glColor3f(0,1,0);
	glVertex2f(-3,-3);
	glColor3f(0,0,1);
	glVertex2f(3,-3);
	glEnd();
	deg += 360.0/60;
}

int main(){
	libui_t* ui = libui_create("test", 0, 0, 400, 300);
	libui_widget_t* w;

	libui_set_draw(ui, draw);

	w = libui_new_widget(ui);
	w->text = "Test";
	w->type = LIBUI_BUTTON;
	w->origin_x = LIBUI_RIGHT;
	w->origin_y = LIBUI_BOTTOM;
	w->x = 0;
	w->y = 0;
	w->width = 75;
	w->height = 25;

	w = libui_new_widget(ui);
	w->type = LIBUI_OPENGL;
	w->origin_x = LIBUI_LEFT;
	w->origin_y = LIBUI_TOP;
	w->x = 0;
	w->y = 0;
	w->width = 300;
	w->height = 300;

	libui_loop(ui);
	libui_destroy(ui);
}
