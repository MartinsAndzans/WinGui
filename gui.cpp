#include "gui.h"

HWND WinGui::hMainWindow = nullptr;

bool WinGui::Init(LPCWSTR WindowTitle, INT32 Width, INT32 Height, COLORREF BackgroundColor, INT nCmdShow) noexcept {
	
	HINSTANCE hInstance = GetModuleHandle(NULL); // * Instance of Application *

	constexpr LPCWSTR ClassName = L"MainClass"; // * Main Window Class *

	constexpr DWORD ClassStyle = CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_GLOBALCLASS; // * Main Window Class Style *
	constexpr DWORD WindowStyle = WS_OVERLAPPEDWINDOW; // * Main Window Style *

	/**************************************/
	/*      Create main window class      */
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = CreateSolidBrush(BackgroundColor);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WindowProcedure;
	wcex.lpszClassName = ClassName;
	wcex.style = ClassStyle;
	/**************************************/

	/**************************************/
	/*     Register main window class     */
	if (!RegisterClassEx(&wcex)) {
		return false;
	}
	/**************************************/

	INT32 X = GetSystemMetrics(SM_CXSCREEN) / 2 - Width / 2; // * Window X Position *
	INT32 Y = GetSystemMetrics(SM_CYSCREEN) / 2 - Height / 2; // * Window Y Position *

	/**************************************/
	/*         Create main window         */
	hMainWindow = CreateWindowEx(NULL,
		ClassName,
		WindowTitle,
		WindowStyle,
		X, Y, Width, Height,
		HWND_DESKTOP,
		NULL,
		hInstance,
		nullptr);
	/**************************************/

	/***************************************/
	/*   Check for window creation error   */
	if (hMainWindow == nullptr) {
		return false;
	}
	/***************************************/

	ShowWindow(hMainWindow, nCmdShow); // # Show Main Window #

	return true;

}

void WinGui::Run(void) noexcept {

	MSG Msg = { 0 }; // * Message *

	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

}

POINT CenterPoint = { 80L, 180L };
BYTE Direction = 0X01;

LRESULT CALLBACK WinGui::WindowProcedure(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	case WM_CREATE:
		SetTimer(hMainWindow, 0x20, 1000U / 60U, nullptr);
		return 0;
	case WM_TIMER:
	{
		if (wParam == 0x20) {

			RECT CanvasSize = { 20L, 20L, 420L, 420L }; // * Canvas Size *
			InvalidateRect(hMainWindow, &CanvasSize, false); // * Prepeare Canvas For Drawing *

			PAINTSTRUCT ps = { 0 };
			HDC WindowDC = BeginPaint(hMainWindow, &ps);

			constexpr int32_t Step = 10; // * Object Speed *
			constexpr int32_t Width = 40; // * Object Width *

			FillRect(WindowDC, &CanvasSize, reinterpret_cast<HBRUSH>(GetClassLongPtr(hMainWindow, GCLP_HBRBACKGROUND))); // # Clear Canvas #

			GdiPlus::DrawRectangle(WindowDC, CanvasSize, ColorU::DarkGreen, 8U); // # Draw Canvas Frame #
			GdiPlus::DrawCircle(WindowDC, CenterPoint, Width, ColorU::DarkGreen, 4U); // # Draw Object #

			if (CenterPoint.x + Width >= CanvasSize.right) {
				Direction = 0;
			} else if (CenterPoint.x - Width <= CanvasSize.left) {
				Direction = 1;
			} else if (CenterPoint.y + Width >= CanvasSize.bottom) {
				Direction = 2;
			} else if (CenterPoint.y - Width <= CanvasSize.top) {
				Direction = 3;
			}

			switch (Direction) {
			case 0:
				CenterPoint.x -= Step;
				CenterPoint.y += Step;
				break;
			case 1:
				CenterPoint.x += Step;
				CenterPoint.y -= Step;
				break;
			case 2:
				CenterPoint.x -= Step;
				CenterPoint.y -= Step;
				break;
			case 3:
				CenterPoint.x += Step;
				CenterPoint.y += Step;
				break;
			}
			
			SetBkMode(WindowDC, TRANSPARENT);
			HGDIOBJ PrevFont = SelectObject(WindowDC, GetStockObject(SYSTEM_FONT));
			SetTextColor(WindowDC, ColorU::DarkGreen);

			std::string Coordinates = "X = " + std::to_string(CenterPoint.x) + " Y = " + std::to_string(CenterPoint.y);
			TextOutA(WindowDC, CanvasSize.left + 20, CanvasSize.top + 20, Coordinates.c_str(), static_cast<int32_t>(Coordinates.length()));

			SelectObject(WindowDC, PrevFont);

			EndPaint(hMainWindow, &ps);
			ValidateRect(hMainWindow, &CanvasSize); // * Save Drawing *

		}
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC WindowDC = BeginPaint(hMainWindow, &ps);
		RECT rect = { 0 };
		GetClientRect(hMainWindow, &rect);

		std::array<COLORREF, 4> Colors = {
			ColorU::Red,
			ColorU::LightGreen,
			ColorU::Blue,
			ColorU::DarkGreen
		};

		GdiPlus::FillGradient(WindowDC, rect.right - 410, rect.bottom - 90, 400, 80, Colors);

		EndPaint(hMainWindow, &ps);
		return 0;
	}
	case WM_COMMAND:
		return 0;
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
