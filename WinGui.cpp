#include "WinGui.h"

//===== Main Window Properties =====//
HWND WinGui::hMainWindow = NULL; // * Handle to Main Window *
SIZE WinGui::MainWindowSize = { 0 }; // * Main Window Size *
COLORREF WinGui::BackgroundColor = 0x00000000; // * Main Window Background Color *
//==================================//

HWND WinGui::hEditBox_Code = NULL;
HWND WinGui::hListBox_Controls = NULL;

bool WinGui::wCreateWindow(_In_ LPCWSTR WindowTitle, _In_ INT32 Width, _In_ INT32 Height, _In_ COLORREF BackgroundColor, _In_ INT nCmdShow) noexcept {
	
	HINSTANCE hInstance = GetModuleHandle(NULL); // * Instance of Application *
	constexpr LPCWSTR ClassName = L"MainClass"; // * Main Window Class *

	WinGui::MainWindowSize = { Width, Height };
	WinGui::BackgroundColor = BackgroundColor;

	/***** Create Main Window Class *****/
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = CreateSolidBrush(WinGui::BackgroundColor);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WindowProc;
	wcex.lpszClassName = ClassName;
	wcex.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_GLOBALCLASS;
	/***** ****** **** ****** ***** *****/

	/***** Register Main Window Class *****/
	if (!RegisterClassExW(&wcex)) {
		return false;
	}
	/***** ******** **** ****** ***** *****/

	INT32 X = GetSystemMetrics(SM_CXSCREEN) / 2 - WinGui::MainWindowSize.cx / 2; // * Window X Position *
	INT32 Y = GetSystemMetrics(SM_CYSCREEN) / 2 - WinGui::MainWindowSize.cy / 2; // * Window Y Position *

	/***** Create Main Window *****/
	WinGui::hMainWindow = CreateWindowExW(NULL,
		ClassName,
		WindowTitle,
		WS_OVERLAPPEDWINDOW,
		X, Y, WinGui::MainWindowSize.cx, WinGui::MainWindowSize.cy,
		HWND_DESKTOP,
		NULL,
		hInstance,
		nullptr);
	/***** ****** **** ****** *****/

	/***** Check for Window Creation Error *****/
	if (WinGui::hMainWindow == NULL) {
		return false;
	}
	/***** ***** *** ****** ******** ***** *****/

	ShowWindow(WinGui::hMainWindow, nCmdShow); // # Show Main Window #

	return true;

}

void WinGui::wRegisterDisplayCallback(DISPLAY DisplayCallback) {



}

void WinGui::wRun(void) noexcept {

	MSG Msg = { 0 }; // * Message Structure *

	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

}

LRESULT CALLBACK WinGui::WindowProc(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	case WM_CREATE:
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);
		hListBox_Controls = CreateWindowExW(WS_EX_STATICEDGE,
			L"ListBox", L"",
			WS_CHILD | WS_VISIBLE | LBS_NOINTEGRALHEIGHT,
			0, 0, 0, 0, hMainWindow, 0, hInstance, nullptr);
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC WindowDC = BeginPaint(hMainWindow, &ps);

		SetBkMode(WindowDC, TRANSPARENT);
		HPEN linePen = CreatePen(PS_SOLID, 6, RGB(145, 0, 0));
		HGDIOBJ PrevPen = SelectObject(WindowDC, linePen);

		std::array<ColorU, 4> colors = {
			ColorU(ColorU::Enum::DarkBlue),
			ColorU(ColorU::Enum::Red),
			ColorU(ColorU::Enum::Green),
			ColorU(0.2F, 0.6F, 1.0F)
		};

		GdiPlus::FillGradient(WindowDC, 10, 10, 400, 40, colors);
		GdiPlus::DrawStar(WindowDC, 40, 40, 200, 200, ColorU(ColorU::Enum::Orange));

		SelectObject(WindowDC, PrevPen);
		DeleteObject(linePen);

		EndPaint(hMainWindow, &ps);
		return 0;
	}
	case WM_COMMAND:
		return 0;
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO minmax = reinterpret_cast<LPMINMAXINFO>(lParam);
		minmax->ptMinTrackSize = { MainWindowSize.cx, MainWindowSize.cy };
		return 0;
	}
	case WM_CLOSE:
		if (MessageBox(hMainWindow, L"You want to exit?", L"-INFORMATION-", MB_ICONINFORMATION | MB_YESNO | MB_DEFBUTTON2) == IDYES)
			DestroyWindow(hMainWindow);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hMainWindow, Msg, wParam, lParam);

}
