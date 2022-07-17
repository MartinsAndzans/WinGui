#include "WinGui.h"

//===== Main Window Properties =====//
HWND WinGui::hMainWindow = NULL; // * Handle to Main Window *
SIZE WinGui::MainWindowSize = { 0 }; // * Main Window Size *
//==================================//

std::unique_ptr<Direct2D1> WinGui::GraphicsDevice;

bool WinGui::wCreateMainWindow(_In_ LPCWSTR WindowTitle, _In_ INT32 Width, _In_ INT32 Height, _In_ INT nCmdShow) noexcept {
	
	HINSTANCE hInstance = GetModuleHandle(NULL); // * Instance of Application *
	constexpr wchar_t ClassName[] = L"DirectX 2D Window Class";
	

	//===== Create Main Window Class =====//
	WNDCLASSEXW wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.hbrBackground = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_CROSS);
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

	WinGui::MainWindowSize = { Width, Height };
	
	INT32 X = GetSystemMetrics(SM_CXSCREEN) / 2 - WinGui::MainWindowSize.cx / 2; // * Window X Position *
	INT32 Y = GetSystemMetrics(SM_CYSCREEN) / 2 - WinGui::MainWindowSize.cy / 2; // * Window Y Position *	

	RECT WindowRect = {
		.left = X,
		.top = Y,
		.right = X + Width,
		.bottom = Y + Height
	};

	WinGui::MainWindowSize = { WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top };

	AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE);

	//======== Create Main Window ========//
	WinGui::hMainWindow = CreateWindowExW(
		WS_EX_APPWINDOW,
		ClassName,
		WindowTitle,
		WS_OVERLAPPEDWINDOW,
		WindowRect.left,
		WindowRect.top,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
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
static Fade state = Fade::out;
static COLORREF Color = D2D1RGB(0, 0, 255);

LRESULT CALLBACK WinGui::WindowProc(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	case WM_CREATE:
	{

		try {
			GraphicsDevice = std::make_unique<Direct2D1>(D2D1_FACTORY_TYPE_SINGLE_THREADED);
		} catch (std::runtime_error &e) {
			MessageBoxA(hMainWindow, e.what(), "DirectX 2D Error", MB_ICONERROR);
			return -1;
		}

		//CreateWindowW(L"Button", L"Open Dialog", WS_CHILD | WS_BORDER | WS_VISIBLE | BS_CENTER,
		//	0, 0, 0, 0, hMainWindow, (HMENU)(0x0001), GetModuleHandle(NULL), nullptr);

		INT16 FrameRate = (INT16)(1000.0F / 60.0F);
		SetTimer(hMainWindow, 0xFF, FrameRate, NULL);

		SYSTEMTIME st = { 0 };
		GetSystemTime(&st);
		srand(st.wMilliseconds);

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

		if (IsIconic(hMainWindow)) {
			return 0;
		}

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

		POINT CursorPos = { 0 };
		GetCursorPos(&CursorPos);
		ScreenToClient(hMainWindow, &CursorPos);

		static std::vector<VERTEX_2F> Stars;
		static std::vector<VERTEX_2F> Spawners;

		/*for (size_t i = 0; i < 2; i++) {
			Stars.push_back({ (FLOAT)(rand() % rect.right), -20.0F });
		}*/

		if (GetAsyncKeyState(VK_LBUTTON) && CursorPos.x >= rect.left && CursorPos.x <= rect.right &&
			CursorPos.y >= rect.top && CursorPos.y <= rect.bottom &&
			GetFocus() == hMainWindow) {
			Stars.push_back({ (FLOAT)(CursorPos.x), (FLOAT)(CursorPos.y) });
		}

		if (GetAsyncKeyState(VK_RBUTTON) && CursorPos.x >= rect.left && CursorPos.x <= rect.right &&
			CursorPos.y >= rect.top && CursorPos.y <= rect.bottom &&
			GetFocus() == hMainWindow) {
			Spawners.push_back({ (FLOAT)(CursorPos.x), (FLOAT)(CursorPos.y) });
		}

		if (GetAsyncKeyState(VK_SPACE) && GetFocus() == hMainWindow) {
			Spawners.clear();
		}

		for (size_t i = 0; i < Spawners.size(); i++) {
			printf_s("X: %.2f\tY: %.2f\n", Spawners[i].x, Spawners[i].y);
			Stars.push_back({ Spawners[i].x, Spawners[i].y });
		}

		for (size_t i = 0; i < Stars.size(); i++) {

			if (Stars[i].y >= rect.bottom) {
				Stars.erase(Stars.begin() + i);
				Stars.shrink_to_fit();
				i--;
				continue;
			}

			switch (rand() % 2) {
			case 0:
				Stars[i].x += 10.0F;
				break;
			case 1:
				Stars[i].x -= 10.0F;
				break;
			}

			Stars[i].y += 10.0F;

		}

		GraphicsDevice->BeginDraw(WindowDC, rect, D2D1::ColorF(Color));

		for (size_t i = 0; i < Stars.size(); i++) {
			D2D1_POINT_2F Point = { Stars[i].x, Stars[i].y};
			GraphicsDevice->FillEllipse(Point, 4.0F, 4.0F, D2D1::ColorF(D2D1RGB(255, 255, 255)));
		}

		GraphicsDevice->EndDraw();

		EndPaint(hMainWindow, &ps);
		
		return 0;
	
	}
	case WM_KEYDOWN:
	{
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
	default:
		return DefWindowProc(hMainWindow, Msg, wParam, lParam);
	}

}
