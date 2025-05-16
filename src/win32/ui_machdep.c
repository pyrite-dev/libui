#include "libui.h"

#include <stdlib.h>
#include <string.h>

void libui_init(void){}

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
			libui_layout(ui);
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
			HDC dc;
			HGLRC glrc;

			memset(&desc, 0, sizeof(desc));
			desc.nSize = sizeof(desc);
			desc.nVersion = 1;
			desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			desc.iPixelType = PFD_TYPE_RGBA;
			desc.cColorBits = 24;
			desc.cAlphaBits = 8;
			desc.cDepthBits = 32;

			w->context = (void*)CreateWindow("OpenGL", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, 0, 0, ui->machdep.window, NULL, ui->machdep.instance, NULL);

			dc = GetDC((HWND)w->context);

			fmt = ChoosePixelFormat(dc, &desc);
			SetPixelFormat(dc, fmt, &desc);

			glrc = wglCreateContext(dc);
		}
	}

	if(w->check_xywh){
		SetWindowPos(w->context, NULL, w->ui_x, w->ui_y, w->ui_width, w->ui_height, 0);
	}
}

void libui_loop(libui_t* ui){
	MSG msg;
	int ret;

	libui_layout(ui);
	libui_process(ui);

	while((ret = GetMessage(&msg, ui->machdep.window, 0, 0)) != 0){
		if(ret == -1) break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		libui_process(ui);
	}
}

void libui_machdep_destroy(libui_t* ui){
	DestroyWindow(ui->machdep.window);
}
