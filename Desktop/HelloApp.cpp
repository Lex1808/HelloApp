#include <Windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <string.h>

static TCHAR szWindowClass[] = _T("SuperApp");
static TCHAR szTitle[] = _T("Windows SuperApp");

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT   message, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, SuperApp!");

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		TextOut(hdc,
			5, 5,
			greeting, _tcslen(greeting));

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	WNDCLASSEXW wind;
	
	wind.cbSize = sizeof(WNDCLASSEX);
	wind.style = CS_HREDRAW | CS_VREDRAW;
	wind.lpfnWndProc = WndProc;
	wind.cbClsExtra = 0;
	wind.cbWndExtra = 0;
	wind.hInstance = hInst;
	wind.hIcon = LoadIcon(wind.hInstance, IDI_APPLICATION);
	wind.hCursor = LoadCursor(NULL, IDC_ARROW);
	wind.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wind.lpszMenuName = NULL;
	wind.lpszClassName = L"SuperApp";
	wind.hIconSm = LoadIcon(wind.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wind)) {
		MessageBox(NULL, _T("Call to RegisterClassEx() failed!"), _T("Windows Learn"), NULL);
		return 1;
	}

	HWND hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		szWindowClass,                      
		szTitle,                            
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,        
		500, 200,                           
		NULL,                              
		NULL,                               
		hInst,                              
		NULL                               
	);

	if (!hWnd) {
		MessageBox(NULL, _T("Call to CreateWindowEx() failed!"), _T("Windows Learn"), NULL);
		return 1;
	}

	ShowWindow(hWnd, cmdshow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;

}
