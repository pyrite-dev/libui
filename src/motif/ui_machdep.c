#include "libui.h"

#include <stdlib.h>
#include <string.h>

#include <Xm/MainW.h>

void libui_init(void){}

libui_t* libui_create(const char* title, int x, int y, int width, int height){
	int argc = 1;
	char** argv = malloc(sizeof(*argv) * 2);
	libui_t* ui = malloc(sizeof(*ui));

	memset(ui, 0, sizeof(*ui));

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
		free(ui);
		return NULL;
	}

	ui->machdep.main = XtVaCreateManagedWidget("main", xmMainWindowWidgetClass, ui->machdep.top, NULL);

	XtMoveWidget(ui->machdep.top, x, y);
	XtResizeWidget(ui->machdep.top, width, height, 1);

	XtRealizeWidget(ui->machdep.top);

	return ui;
}

void libui_loop(libui_t* ui){
	XEvent ev;
	while(1){
		XtAppNextEvent(ui->machdep.context, &ev);
		XtDispatchEvent(&ev);
	}
}

void libui_destroy(libui_t* ui){
	XtDestroyWidget(ui->machdep.top);
	free(ui);
}
