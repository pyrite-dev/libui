#include "libui.h"

#include <stdlib.h>
#include <string.h>

void libui_init(void){}

LRESULT CALLBACK libui_wndproc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp){
	return DefWindowProc(wnd, msg, wp, lp);
}

libui_t* libui_create(const char* title, int x, int y, int width, int height){
	WNDCLASSEX wc;
	libui_t* ui = malloc(sizeof(*ui));
	memset(ui, 0, sizeof(*ui));

	ui->machdep.instance = (HINSTANCE)GetModuleHandle(NULL);

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = libui_wndproc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ui->machdep.instance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_MENU);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = title;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	if(!RegisterClassEx(&wc)){
		free(ui);
		return NULL;
	}

	ui->machdep.window = CreateWindow(title, title, WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, 0, ui->machdep.instance, NULL);
	if(ui->machdep.window == NULL){
		free(ui);
		return NULL;
	}

	ShowWindow(ui->machdep.window, SW_NORMAL);
	UpdateWindow(ui->machdep.window);

	return ui;
}

void libui_loop(libui_t* ui){
	MSG msg;
	int ret;
	while((ret = GetMessage(&msg, ui->machdep.window, 0, 0)) != 0){
		if(ret == -1) break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void libui_destroy(libui_t* ui){
	DestroyWindow(ui->machdep.window);
	free(ui);
}
