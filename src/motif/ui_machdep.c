#include "libui.h"

#include <stdlib.h>
#include <string.h>

#include <Xm/MainW.h>

void libui_init(void){}

int libui_machdep_create(libui_t* ui, const char* title, int x, int y, int width, int height){
	int argc = 1;
	char** argv = malloc(sizeof(*argv) * 2);

	argv[0] = malloc(6);
	argv[1] = NULL;

	strcpy(argv[0], "dummy");

	ui->machdep.top = XtVaAppInitialize(
		&ui->machdep.context,
		title,
		NULL, 0,
		&argc, argv,
		NULL,
		XtNtitle, title,
	NULL);

	if(ui->machdep.top == NULL){
		return -1;
	}

	ui->machdep.main = XtVaCreateManagedWidget("main", xmMainWindowWidgetClass, ui->machdep.top, NULL);

	XtMoveWidget(ui->machdep.top, x, y);
	XtResizeWidget(ui->machdep.top, width, height, 1);

	XtRealizeWidget(ui->machdep.top);

	ui->machdep.wmdel = XInternAtom(XtDisplay(ui->machdep.top), "WM_DELETE_WINDOW", False);
	XSetWMProtocols(XtDisplay(ui->machdep.top), XtWindow(ui->machdep.top), &ui->machdep.wmdel, 1);

	ui->x = x;
	ui->y = y;
	ui->width = width;
	ui->height = height;

	return 0;
}

void libui_loop(libui_t* ui){
	XEvent ev;
	while(1){
		XtAppNextEvent(ui->machdep.context, &ev);
		if(ev.type == ClientMessage && ev.xclient.data.l[0] == ui->machdep.wmdel){
			continue;
		}
		XtDispatchEvent(&ev);
		if(ev.type == ResizeRequest || ev.type == ConfigureNotify){
			XtWidgetGeometry geo;
			int trig = 0;
			XtQueryGeometry(ui->machdep.top, NULL, &geo);
			if(ui->x != geo.x || ui->y != geo.y || ui->width != geo.width || geo.height) trig = 1;
			ui->x = geo.x;
			ui->y = geo.y;
			ui->width = geo.width;
			ui->height = geo.height;

			if(trig) libui_layout(ui);
		}
	}
}

void libui_machdep_destroy(libui_t* ui){
	XtDestroyWidget(ui->machdep.top);
}
