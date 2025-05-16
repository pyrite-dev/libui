#include <stdio.h>

#include <libui.h>

libui_widget_t* opengl;
libui_widget_t* btn;

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

void resize(libui_t* ui, int width, int height){
	btn->width = 75;
	btn->height = 25;
	btn->x = width / 2 - btn->width / 2;

	opengl->width = opengl->height = (width > (height - btn->height - 15)) ? (height - btn->height - 15) : (width);
	opengl->x = width / 2 - opengl->width / 2;
}

int main(){
	libui_t* ui = libui_create("test", 0, 0, 400, 400);
	libui_widget_t* w;

	libui_set_draw(ui, draw);
	libui_set_resize(ui, resize);

	w = libui_new_widget(ui);
	w->text = "Test";
	w->type = LIBUI_BUTTON;
	w->origin_x = LIBUI_LEFT;
	w->origin_y = LIBUI_BOTTOM;
	btn = w;

	w = libui_new_widget(ui);
	w->type = LIBUI_OPENGL;
	w->origin_x = LIBUI_LEFT;
	w->origin_y = LIBUI_TOP;
	opengl = w;

	libui_loop(ui);
	libui_destroy(ui);
}
