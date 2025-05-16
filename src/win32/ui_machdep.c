#include "libui.h"

#include <stdlib.h>
#include <string.h>

#include <stb_ds.h>

void libui_init(void){}

typedef struct machdep_ {
	HDC dc;
	HGLRC glrc;
} machdep_t;

LRESULT CALLBACK libui_wndproc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp){
	libui_t* ui = (libui_t*)GetWindowLongPtr(wnd, GWLP_USERDATA);

	switch(msg){
		case WM_SIZE: {
			RECT rect;
			GetClientRect(wnd, &rect);
			ui->x = rect.left;
			ui->y = rect.top;
			ui->width = rect.right - rect.left;
			ui->height = rect.bottom - rect.top;
			if(ui->resize != NULL) ui->resize(ui, ui->width, ui->height);
			libui_layout(ui);
			libui_process(ui);
			break;
		}
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(wnd, &ps);
			EndPaint(wnd, &ps);
			break;
		}
		default: {
			return DefWindowProc(wnd, msg, wp, lp);
		}
	}
	return 0;
}

int libui_machdep_create(libui_t* ui, const char* title, int x, int y, int width, int height){
	WNDCLASSEX wc;
	RECT rect;
	DWORD style;

	ui->machdep.instance = (HINSTANCE)GetModuleHandle(NULL);

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = libui_wndproc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ui->machdep.instance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(0x80, 0x80, 0x80));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = title;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	if(!RegisterClassEx(&wc)){
		return -1;
	}

	ui->machdep.window = CreateWindow(title, title, WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, 0, ui->machdep.instance, NULL);
	if(ui->machdep.window == NULL){
		return -1;
	}
	SetWindowLongPtr(ui->machdep.window, GWLP_USERDATA, (LONG_PTR)ui);

	SetRect(&rect, 0, 0, width, height);
	style = (DWORD)GetWindowLongPtr(ui->machdep.window, GWL_STYLE);
	AdjustWindowRect(&rect, style, FALSE);
	SetWindowPos(ui->machdep.window, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE);

	ShowWindow(ui->machdep.window, SW_NORMAL);
	UpdateWindow(ui->machdep.window);

	return 0;
}

void libui_machdep_process(libui_t* ui, libui_widget_t* w){
	if(w->context == NULL){
		if(w->type == LIBUI_BUTTON){
			w->context = CreateWindow("BUTTON", w->text == NULL ? "(not set)" : w->text, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 0, 0, ui->machdep.window, NULL, ui->machdep.instance, NULL);
		}else if(w->type == LIBUI_OPENGL){
			PIXELFORMATDESCRIPTOR desc;
			int fmt;
			machdep_t* m;

			memset(&desc, 0, sizeof(desc));
			desc.nSize = sizeof(desc);
			desc.nVersion = 1;
			desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			desc.iPixelType = PFD_TYPE_RGBA;
			desc.cColorBits = 24;
			desc.cAlphaBits = 8;
			desc.cDepthBits = 32;

			w->context = (void*)CreateWindow("Static", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, 0, 0, ui->machdep.window, NULL, ui->machdep.instance, NULL);

			w->machdep = malloc(sizeof(machdep_t));
			m = (machdep_t*)w->machdep;

			m->dc = GetDC((HWND)w->context);

			fmt = ChoosePixelFormat(m->dc, &desc);
			SetPixelFormat(m->dc, fmt, &desc);

			m->glrc = wglCreateContext(m->dc);
		}
	}

	if(w->check_xywh){
		SetWindowPos(w->context, NULL, w->ui_x, w->ui_y, w->ui_width, w->ui_height, 0);
		if(w->type == LIBUI_OPENGL){
			machdep_t* m = (machdep_t*)w->machdep;
			wglMakeCurrent(m->dc, m->glrc);
			glViewport(0, 0, w->width, w->height);
		}
	}
}

void libui_loop(libui_t* ui){
	MSG msg;
	int ret;

	libui_layout(ui);
	libui_process(ui);

	while((ret = PeekMessage(&msg, ui->machdep.window, 0, 0, PM_NOREMOVE)) != -1){
		if(!ret && ui->draw != NULL){
			int i;
			for(i = 0; i < arrlen(ui->widgets); i++){
				if(ui->widgets[i]->type == LIBUI_OPENGL){
					machdep_t* m = (machdep_t*)ui->widgets[i]->machdep;
					wglMakeCurrent(m->dc, m->glrc);
					ui->draw(ui, ui->widgets[i]);
					SwapBuffers(m->dc);	
				}
			}
			continue;
		}
		
		if(GetMessage(&msg, ui->machdep.window, 0, 0) == -1) break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		libui_process(ui);
	}
}

void libui_machdep_destroy(libui_t* ui){
	while(arrlen(ui->widgets) > 0){
		if(ui->widgets[0]->machdep != NULL){
			if(ui->widgets[0]->type == LIBUI_OPENGL){
				machdep_t* m = (machdep_t*)ui->widgets[0]->machdep;
				wglMakeCurrent(NULL, NULL);
				DeleteDC(m->dc);
				wglDeleteContext(m->glrc);
			}
			free(ui->widgets[0]->machdep);
		}
		if(ui->widgets[0]->context != NULL){
			DestroyWindow((HWND)ui->widgets[0]->context);
		}
		free(ui->widgets[0]);
		arrdel(ui->widgets, 0);
	}
	arrfree(ui->widgets);
	DestroyWindow(ui->machdep.window);
}
