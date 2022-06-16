#include "WinGui.h"

/***********************************************/
/*           Main Window Properties            */
HWND WinGui::hMainWindow = nullptr;
SIZE WinGui::MainWindowSize = { 0 };
/***********************************************/

HWND WinGui::hTestBtn = nullptr;

bool WinGui::CreateMainWindow(
	_In_ LPCWSTR WindowTitle,
	_In_ INT32 Width,
	_In_ INT32 Height,
	_In_ COLORREF BackgroundColor,
	_In_ LPCWSTR IconResource,
	_In_ LPWSTR CursorResource,
	_In_ INT nCmdShow
) noexcept {
	
	HINSTANCE hInstance = GetModuleHandle(NULL); // * Instance of Application *
	constexpr LPCWSTR ClassName = L"MainClass"; // * Main Window Class *

	MainWindowSize = { Width, Height };

	/**************************************/
	/*      Create main window class      */
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = CreateSolidBrush(BackgroundColor);
	wcex.hCursor = LoadCursor(hInstance, CursorResource);
	wcex.hIcon = LoadIcon(hInstance, IconResource);
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WindowProcedure;
	wcex.lpszClassName = ClassName;
	wcex.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_GLOBALCLASS;
	/**************************************/

	/**************************************/
	/*     Register main window class     */
	if (!RegisterClassEx(&wcex)) {
		return false;
	}
	/**************************************/

	INT32 X = GetSystemMetrics(SM_CXSCREEN) / 2 - MainWindowSize.cx / 2; // * Window X Position *
	INT32 Y = GetSystemMetrics(SM_CYSCREEN) / 2 - MainWindowSize.cy/ 2; // * Window Y Position *

	/**************************************/
	/*         Create main window         */
	hMainWindow = CreateWindowEx(NULL,
		ClassName,
		WindowTitle,
		WS_OVERLAPPEDWINDOW,
		X, Y, MainWindowSize.cx, MainWindowSize.cy,
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
RECT CanvasSize = { 20L, 20L, 420L, 420L }; // * Canvas Size *

LRESULT CALLBACK WinGui::WindowProcedure(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	case WM_CREATE:
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);
		hTestBtn = CreateWindowEx(WS_EX_STATICEDGE, L"Button", L"Test", WS_CHILD | WS_VISIBLE | BS_VCENTER, 0, 0, 0, 0, hMainWindow, reinterpret_cast<HMENU>(0x22), hInstance, nullptr);
		return 0;
	}
	case WM_SIZE:
	{
		SIZE MainWindowSize = { LOWORD(lParam), HIWORD(lParam) };
		SetWindowPos(hTestBtn, nullptr, MainWindowSize.cx - 210, 10, 200, 40, SWP_SHOWWINDOW);
		return 0;
	}
	case WM_TIMER:
	{
		if (wParam == 0x20) {

			InvalidateRect(hMainWindow, &CanvasSize, false); // * Prepeare Canvas For Drawing *

			PAINTSTRUCT ps = { 0 };
			HDC WindowDC = BeginPaint(hMainWindow, &ps);

			constexpr int32_t Step = 10; // * Object Speed *
			constexpr int32_t Radius = 40; // * Object Width *

			GdiPlus::FillRectangle(WindowDC, CanvasSize, ColorU::LightBlue); // # Clear Canvas #
			GdiPlus::DrawRectangle(WindowDC, CanvasSize, ColorU::DarkBlue, 8U); // # Draw Canvas Frame #
			GdiPlus::DrawCircle(WindowDC, CenterPoint, Radius, ColorU::DarkBlue, 4U); // # Draw Object #
			GdiPlus::DrawRectangle(WindowDC, CenterPoint.x - Radius, CenterPoint.y - Radius, Radius * 2, Radius * 2, ColorU::Black, 2U);

			if (CenterPoint.x + Radius >= CanvasSize.right) {
				Direction = 0;
			} else if (CenterPoint.x - Radius <= CanvasSize.left) {
				Direction = 1;
			} else if (CenterPoint.y + Radius >= CanvasSize.bottom) {
				Direction = 2;
			} else if (CenterPoint.y - Radius <= CanvasSize.top) {
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
			HGDIOBJ PrevFont = SelectObject(WindowDC, GetStockObject(SYSTEM_FIXED_FONT));
			SetTextColor(WindowDC, ColorU::DarkBlue);

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
		
		RECT rect = { 0 };
		GetClientRect(hMainWindow, &rect);
		
		PAINTSTRUCT ps;
		HDC WindowDC = BeginPaint(hMainWindow, &ps);
		GdiPlus::FillRectangle(WindowDC, rect, reinterpret_cast<HBRUSH>(GetClassLongPtr(hMainWindow, GCLP_HBRBACKGROUND)));
		EndPaint(hMainWindow, &ps);

		return 0;
	
	}
	case WM_MOUSEWHEEL:
	{
		SHORT WheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		if (WheelDelta > 0i16) {
			CanvasSize.right += 10;
			CanvasSize.bottom += 10;
		} else if (WheelDelta < 0i16) {
			CanvasSize.right -= 10;
			CanvasSize.bottom -= 10;
		}

		RedrawWindow(hMainWindow, nullptr, nullptr, RDW_INTERNALPAINT | RDW_INVALIDATE);
	}
	case WM_COMMAND:
	{
		
		switch (LOWORD(wParam)) {
		case 0x22:
		{
			SetTimer(hMainWindow, 0x20, 1000U / 60U, nullptr);
			GdiPlus::ShowDialog(hMainWindow, "");
			break;
		}
		}
		
		return 0;

	}
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
