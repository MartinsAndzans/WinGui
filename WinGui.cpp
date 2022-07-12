#include "WinGui.h"

//===== Main Window Properties =====//
HWND WinGui::hMainWindow = NULL; // * Handle to Main Window *
SIZE WinGui::MainWindowSize = { 0 }; // * Main Window Size *
//==================================//

Direct2D1 *WinGui::GraphicsDevice = nullptr;

bool WinGui::wCreateMainWindow(_In_ LPCWSTR WindowTitle, _In_ INT32 Width, _In_ INT32 Height, _In_ INT nCmdShow) noexcept {
	
	HINSTANCE hInstance = GetModuleHandle(NULL); // * Instance of Application *
	constexpr LPCWSTR ClassName = L"MainClass"; // * Main Window Class *

	WinGui::MainWindowSize = { Width, Height };

	//===== Create Main Window Class =====//
	WNDCLASSEXW wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.hbrBackground = NULL;
	wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_MAINCURSOR));
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WindowProc;
	wcex.lpszClassName = ClassName;
	wcex.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	//====================================//

	//==== Register Main Window Class ====//
	if (!RegisterClassExW(&wcex)) {
		return false;
	}
	//====================================//

	INT32 X = GetSystemMetrics(SM_CXSCREEN) / 2 - WinGui::MainWindowSize.cx / 2; // * Window X Position *
	INT32 Y = GetSystemMetrics(SM_CYSCREEN) / 2 - WinGui::MainWindowSize.cy / 2; // * Window Y Position *

	//======== Create Main Window ========//
	WinGui::hMainWindow = CreateWindowExW(
		WS_EX_APPWINDOW,
		ClassName,
		WindowTitle,
		WS_OVERLAPPEDWINDOW,
		X, Y, WinGui::MainWindowSize.cx, WinGui::MainWindowSize.cy,
		HWND_DESKTOP,
		NULL,
		hInstance,
		nullptr
	);
	//====================================//

	//===== Check For Window Creation Error =====//
	if (WinGui::hMainWindow == NULL) {
		return false;
	}
	//===========================================//

	ShowWindow(WinGui::hMainWindow, nCmdShow); // # Show Main Window #

	return true;

}

INT WinGui::wMainLoop(void) noexcept {

	MSG Msg = { 0 }; // * Message Structure *
	
	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return static_cast<INT>(Msg.wParam);

}

enum class Fade { out, in };
Fade state = Fade::out;
COLORREF Color = D2D1RGB(0, 0, 255);

BOOL Reset = FALSE;

LRESULT CALLBACK WinGui::WindowProc(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	case WM_CREATE:
	{

		try {
			GraphicsDevice = new Direct2D1(D2D1_FACTORY_TYPE_SINGLE_THREADED);
		} catch (std::runtime_error &e) {
			MessageBoxA(hMainWindow, e.what(), "DirectX 2D Error", MB_ICONERROR);
			return -1;
		}

		//CreateWindowW(L"Button", L"Open Dialog", WS_CHILD | WS_BORDER | WS_VISIBLE | BS_CENTER,
		//	0, 0, 0, 0, hMainWindow, (HMENU)(0x0001), GetModuleHandle(NULL), nullptr);

		INT16 FrameRate = (INT16)(1000.0F / 60.0F);
		SetTimer(hMainWindow, 0xFF, FrameRate, NULL);

		return 0;
		
	}
	case WM_SIZE:
	{
		//HWND DlgBtn = GetDlgItem(hMainWindow, 0x0001);
		//SetWindowPos(DlgBtn, NULL, LOWORD(lParam) - 210, 10, 200, 40, SWP_SHOWWINDOW);
	}
	case WM_TIMER:
	{

		if (Color == D2D1RGB(0, 0, 0)) {
			state = Fade::in;
		} else if (Color == D2D1RGB(0, 0, 255)) {
			state = Fade::out;
		}

		InvalidateRect(hMainWindow, NULL, FALSE);

		return 0;
	}
	case WM_PAINT:
	{

		RECT rect = { 0 };
		PAINTSTRUCT ps = { 0 };

		HDC WindowDC = BeginPaint(hMainWindow, &ps);
		GetClientRect(hMainWindow, &rect);

		SetBkMode(WindowDC, TRANSPARENT);

		if (state == Fade::out) {
			Color = D2D1RGB(D2D1GetRValue(Color), D2D1GetGValue(Color), D2D1GetBValue(Color) - 0x05);
		} else if (state == Fade::in) {
			Color = D2D1RGB(D2D1GetRValue(Color), D2D1GetGValue(Color), D2D1GetBValue(Color) + 0x05);
		}

		enum class MOVE { UP, DOWN };

		static FLOAT Speed = 0.0F, Acelaration = 2.0F;
		static D2D1_POINT_2F circle_point = { rect.right / 2.0F, rect.bottom / 4.0F };
		static MOVE state = MOVE::DOWN;

		if (Reset == TRUE) {
			Speed = 0.0F;
			circle_point = { rect.right / 2.0F, rect.bottom / 4.0F };
			state = MOVE::DOWN;
			Reset = FALSE;
		}

		if (state == MOVE::DOWN) {
			circle_point.y += Speed;
			Speed += Acelaration;
		} else if(state == MOVE::UP) {
			circle_point.y -= Speed;
			Speed -= Acelaration * 1.4F;
		}

		if (circle_point.y + 100.0F - 10.0F >= rect.bottom) {
			state = MOVE::UP;
		} else if (Speed <= 0.0F) {
			state = MOVE::DOWN;
		}

		std::vector<VERTEX> Rect = {
			VERTEX(10.0F, 10.0F),
			VERTEX((FLOAT)(rect.right) - 10.0F, 10.0F),
			VERTEX((FLOAT)(rect.right) - 10.0F, (FLOAT)(rect.bottom) - 10.0F),
			VERTEX(10.0F, (FLOAT)(rect.bottom) - 10.0F)
		};

		GraphicsDevice->BeginDraw(WindowDC, rect, D2D1::ColorF(Color));

		GraphicsDevice->DrawEllipse(circle_point, 100.0F, 100.0F, D2D1::ColorF(D2D1::ColorF::LightSkyBlue), 10.0F, MODE::FILL);
		GraphicsDevice->DrawGeometry(Rect, D2D1::ColorF(D2D1RGB(0, 145, 0)), 4.0F, MODE::DRAW);

		GraphicsDevice->EndDraw();

		EndPaint(hMainWindow, &ps);
		
		return 0;
	
	}
	case WM_KEYDOWN:
	{
		if (wParam == VK_SPACE) {
			Reset = TRUE;
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
		delete GraphicsDevice;
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hMainWindow, Msg, wParam, lParam);
	}

}
