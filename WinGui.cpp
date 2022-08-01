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

struct Component {
	VERTEX_2F Position;
	SIZE_2F Size;
	FLOAT Rotation;
};

struct Clicks {
	VERTEX_2F Center;
	FLOAT Radius;
};

LRESULT CALLBACK WinGui::WindowProc(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam) {

	enum class Fade { out, in };
	static Fade state = Fade::out;
	static COLORREF Color = D2D1RGB(0, 0, 255);

	static VERTEX_2F CursorPos;
	static bool LMButtonPresed = false;

	switch (Msg) {
	case WM_CREATE:
	{

		try {
			GraphicsDevice = std::make_unique<Direct2D1>(D2D1_FACTORY_TYPE_MULTI_THREADED);
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
	case WM_MOUSEMOVE:
	{
		CursorPos.x = LOWORD(lParam);
		CursorPos.y = HIWORD(lParam);
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

		static std::vector<Component> Stars;
		static std::vector<Clicks> Clicks;

		if (LMButtonPresed) {
			Clicks.push_back({ CursorPos, 80.0F });
		}

		for (size_t i = 0; i < Clicks.size(); i++) {
			
			if (Clicks[i].Radius <= 0.0F) {
				Clicks.erase(Clicks.begin() + i);
				i -= 1;
				continue;
			}

			Clicks[i].Radius -= 2.0F;
		
		}

		// # SPAWN #
		for (size_t i = 0; i < 2; i++) {
			if (Stars.size() <= 40) {
				FLOAT Size = (FLOAT)(rand() % 41 + 10);
				Stars.push_back({ { -20.0F, (FLOAT)(rand() % rect.bottom) }, { Size, Size }, 0.0F });
			}
		}

		// # ANIMATION #
		for (size_t i = 0; i < Stars.size(); i++) {

			if (Stars[i].Position.x >= rect.right) {
				Stars.erase(Stars.begin() + i);
				Stars.shrink_to_fit();
				i -= 1;
				continue;
			}

			// # SPEED #
			if (Stars[i].Size.width <= 18) {
				Stars[i].Position.x += 28.0F;
			} else if (Stars[i].Size.width <= 28) {
				Stars[i].Position.x += 18.0F;
			} else {
				Stars[i].Position.x += 8.0F;
			}

			// # ROTATION #
			if (Stars[i].Rotation >= 360.0F) {
				Stars[i].Rotation = 0.0F;
			} else {
				Stars[i].Rotation += (FLOAT)(rand() % 11 + 2);
			}

		}

		GraphicsDevice->BeginDraw(WindowDC, rect, D2D1::ColorF(D2D1RGB(0, 0, 0)));

		for (size_t i = 0; i < Stars.size(); i++) {

			std::vector<VERTEX_2F> StarVertex;
			StarVertex.reserve(5);
			StarVertex.push_back({ Stars[i].Position.x, Stars[i].Position.y + Stars[i].Size.height });
			StarVertex.push_back({ Stars[i].Position.x + Stars[i].Size.width / 2.0F, Stars[i].Position.y });
			StarVertex.push_back({ Stars[i].Position.x + Stars[i].Size.width, Stars[i].Position.y + Stars[i].Size.height });
			StarVertex.push_back({ Stars[i].Position.x, Stars[i].Position.y + Stars[i].Size.height / 3.0F });
			StarVertex.push_back({ Stars[i].Position.x + Stars[i].Size.width, Stars[i].Position.y + Stars[i].Size.height / 3.0F });
			
			D2D1_MATRIX_3X2_F Rotate;
			D2D1MakeRotateMatrix(Stars[i].Rotation, { Stars[i].Position.x + Stars[i].Size.width / 2.0F, Stars[i].Position.y + Stars[i].Size.height / 2.0F }, &Rotate);

			GraphicsDevice->DrawGeometry(StarVertex, D2D1::ColorF(D2D1::ColorF::Enum::Yellow), 2.0F, &Rotate);

		}

		for (size_t i = 0; i < Clicks.size(); i++) {
			GraphicsDevice->DrawEllipse(Clicks[i].Center, Clicks[i].Radius, Clicks[i].Radius, D2D1::ColorF(D2D1::ColorF::Enum::DarkOrange), 2.0F);
		}

		GraphicsDevice->EndDraw();

		EndPaint(hMainWindow, &ps);

		return 0;

	}
	case WM_LBUTTONDOWN:
	{
		LMButtonPresed = true;
		return 0;
	}
	case WM_LBUTTONUP:
	{
		LMButtonPresed = false;
		return 0;
	}
	case WM_KEYDOWN:
	{

		if (wParam == VK_ESCAPE) {
			PostMessage(hMainWindow, WM_CLOSE, 0, 0);
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
	default:
		return DefWindowProc(hMainWindow, Msg, wParam, lParam);
	}

}
