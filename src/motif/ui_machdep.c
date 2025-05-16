#include "libui.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <stb_ds.h>

#include <Xm/MainW.h>
#include <Xm/PushB.h>

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

	ui->machdep.main = XtVaCreateManagedWidget("main", xmMainWindowWidgetClass, ui->machdep.top,
		XmNbackground, 0x808080,
	NULL);

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

void libui_machdep_process(libui_t* ui, libui_widget_t* w){
	if(w->context == NULL){
		XftFont* font = XftFontOpenName(XtDisplay(ui->machdep.top), DefaultScreen(ui->machdep.top), "sans:size=12");
		Arg ft[2];

		if(w->type == LIBUI_BUTTON){
			char buf[512];
			XmString str = XmStringCreateLocalized(w->text == NULL ? "(not set)" : w->text);
			sprintf(buf, "id%d", w->id);
			w->context = (void*)XtVaCreateWidget(buf, xmPushButtonWidgetClass, ui->machdep.main, XmNlabelString, str, NULL);
			XtManageChild((Widget)w->context);
			XtUnmanageChild((Widget)w->context);
			XtMapWidget((Widget)w->context);
			XmStringFree(str);
		}

		if(font != NULL){
			XmRendition r;
			XmRenderTable rt;
			XtSetArg(ft[0], XmNfontType, XmFONT_IS_XFT);
			XtSetArg(ft[1], XmNxftFont, font);
			r = XmRenditionCreate((Widget)w->context, XmFONTLIST_DEFAULT_TAG, ft, 2);
			rt = XmRenderTableAddRenditions(NULL, &r, 1, XmMERGE_REPLACE);
			XtVaSetValues((Widget)w->context, XmNrenderTable, rt, NULL);
		}
	}

	if(w->check_xywh){
		Widget ctx = (Widget)w->context;
		XtVaSetValues(ctx, XmNx, w->ui_x, XmNy, w->ui_y, XmNwidth, w->ui_width, XmNheight, w->ui_height, NULL);
	}
}

void libui_loop(libui_t* ui){
	XEvent ev;

	libui_layout(ui);
	libui_process(ui);

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
		libui_process(ui);
	}
}

void libui_machdep_destroy(libui_t* ui){
	while(arrlen(ui->widgets) > 0){
		if(ui->widgets[0]->context != NULL){
			XtDestroyWidget((Widget)ui->widgets[0]->context);
		}
		free(ui->widgets[0]);
		arrdel(ui->widgets, 0);
	}
	arrfree(ui->widgets);
	XtDestroyWidget(ui->machdep.top);
}
